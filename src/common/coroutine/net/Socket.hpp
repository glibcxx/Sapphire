#pragma once

#include <algorithm>
#include <coroutine>
#include <cstdint>
#include <new>
#include <ranges>
#include <span>
#include <stop_token>
#include <type_traits>

#include "common/Expected.hpp"
#include "common/sys/MiniWindows.h"
#include "IPEndpoint.hpp"
#include "NetOperation.hpp"
#include "NetResult.hpp"
#include "SocketHandle.hpp"
#include "SocketInit.hpp" // IWYU pragma: keep
#include "../IoContext.hpp"

namespace sapphire::coro::net {

    class [[nodiscard]] Socket {
        win::SocketHandle mHandle;
        IPEndpoint        mLocalEndPoint;
        IPEndpoint        mRemoteEndPoint;

        enum class Protocol : std::uint8_t { TCP,
                                             UDP };

        static NetResult<win::SocketHandle>
        createSocket(int addressFamily, int socketType, int protocol, IoContext &ctx) {
            win::SocketHandle socketHandle = WSASocketW(
                addressFamily, socketType, protocol, NULL, 0, 0x01 | 0x80 // WSA_FLAG_OVERLAPPED | WSA_FLAG_NO_HANDLE_INHERIT
            );
            if (!socketHandle.isValid()) return {unexpected, GetLastError(), "WSASocketW"};

            if (!ctx.attach((sys::win::handle_t)socketHandle.get())) return {unexpected, GetLastError(), "CreateIoCompletionPort"};

            const unsigned char flags = 0x1 | 0x2; // FILE_SKIP_COMPLETION_PORT_ON_SUCCESS | FILE_SKIP_SET_EVENT_ON_HANDLE
            if (!SetFileCompletionNotificationModes((sys::win::handle_t)socketHandle.get(), flags))
                return {unexpected, GetLastError(), "SetFileCompletionNotificationModes"};

            if (socketType == 1 /* SOCK_STREAM*/) {
                sys::win::bool_t value = 1;
                const int        result = setsockopt(
                    socketHandle.get(), 0xffff /*SOL_SOCKET*/, (int)(~0x0080) /*SO_DONTLINGER*/, reinterpret_cast<const char *>(&value), sizeof(value)
                );
                if (result == -1)
                    return {unexpected, WSAGetLastError(), "SetFileCompletionNotificationModes"};
            }

            return socketHandle;
        }

        template <typename T, bool Const = false>
        static constexpr bool isConvertibleToCharSpan =
            std::is_constructible_v<std::span<char>, T> || std::is_constructible_v<std::span<unsigned char>, T>
            || std::is_constructible_v<std::span<signed char>, T>;

        template <typename T>
        static constexpr bool isConvertibleToCharSpan<T, true> =
            std::is_constructible_v<std::span<const char>, T>
            || std::is_constructible_v<std::span<const unsigned char>, T>
            || std::is_constructible_v<std::span<const signed char>, T>;

    public:
        explicit Socket(win::SocketHandle handle) noexcept : mHandle(std::move(handle)) {}

        win::SocketHandle       &getSocketHandle() noexcept { return mHandle; }
        const win::SocketHandle &getSocketHandle() const noexcept { return mHandle; }

        static NetResult<Socket> createTCPv4(coro::IoContext &ctx) {
            return {createSocket(2, 1, 6, ctx)};
        }
        static NetResult<Socket> createTCPv6(coro::IoContext &ctx) {
            return {createSocket(23, 1, 6, ctx)};
        }
        static NetResult<Socket> createUDPv4(coro::IoContext &ctx) {
            return {createSocket(2, 2, 17, ctx)};
        }
        static NetResult<Socket> createUDPv6(coro::IoContext &ctx) {
            return {createSocket(23, 2, 17, ctx)};
        }

        NetResult<void> bind(const IPEndpoint &localEp) {
            coro::win::sockaddr_storage sockaddrStorage{};

            coro::win::sockaddr &sockaddr = reinterpret_cast<coro::win::sockaddr &>(sockaddrStorage);
            int                  namelen = localEp.toSockAddr(sockaddr);

            int result = ::bind(mHandle.get(), (struct sockaddr *)&sockaddr, namelen);
            if (result != 0) return {unexpected, WSAGetLastError(), "bind(localEp)"};

            int sockaddrLen = sizeof(sockaddrStorage);
            result = getsockname(mHandle.get(), (struct sockaddr *)&sockaddr, &sockaddrLen);
            mLocalEndPoint = result == 0 ? IPEndpoint::fromSockAddr(sockaddr) : localEp;
            return {};
        }

        NetResult<void> listen() {
            int result = ::listen(mHandle.get(), 0x7fffffff);
            if (result == -1) return {unexpected, WSAGetLastError(), "listen()"};
            return {};
        }

        void close() noexcept { mHandle.close(); }

        void abort() noexcept {
            if (mHandle.isValid()) {
                coro::win::linger linger;
                linger.l_onoff = 1;
                linger.l_linger = 0;
                setsockopt(mHandle.get(), 0xffff /*SOL_SOCKET*/, 0x0080 /*SO_LINGER*/, (const char *)&linger, sizeof(linger));
                mHandle.close();
            }
        }

        NetResult<void> closeSend() {
            int result = shutdown(mHandle.get(), 0x01);
            if (result == -1) return {unexpected, WSAGetLastError(), "shutdown(SD_SEND)"};
            return {};
        }

        NetResult<void> closeRecv() {
            int result = shutdown(mHandle.get(), 0x00);
            if (result == -1) return {unexpected, WSAGetLastError(), "shutdown(SD_RECEIVE)"};
            return {};
        }

        struct [[nodiscard]] DisconnectOperation : NetOperation {
            Socket &mSocket;
            DisconnectOperation(Socket &s) noexcept : mSocket(s) {}
            win::socket_t getCancelableHandle() const noexcept { return mSocket.getSocketHandle().get(); }
            bool          await_suspend(std::coroutine_handle<> h) noexcept {
                using LPFN_DISCONNECTEX = sys::win::winbool_t(__stdcall *)(unsigned __int64 s, LPOVERLAPPED lpOverlapped, sys::win::dword_t dwFlags, sys::win::dword_t dwReserved);
                this->mContinuation = h;
                auto lpfnDisconnectEx = getExFuncPtr<LPFN_DISCONNECTEX>(
                    mSocket.mHandle,
                    {
                        0x7fda2e11, 0x8630, 0x436f, {0xa0, 0x31, 0xf5, 0x36, 0xa6, 0xee, 0xc1, 0x57}
                }
                );
                if (!lpfnDisconnectEx) return false;
                bool ok = lpfnDisconnectEx(mSocket.mHandle.get(), reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(this)), 0, 0);
                return handleIoResult(ok);
            }
            NetResult<void> await_resume() noexcept {
                if (mErrorCode != ERROR_SUCCESS) return {unexpected, (int)mErrorCode, "DisconnectEx"};
                return {};
            }
        };

        auto disconnect() noexcept { return DisconnectOperation{*this}; }

        struct [[nodiscard]] AcceptOperation : NetOperation {
            Socket   &mListeningSocket;
            Socket   &mAcceptingSocket;
            std::byte mBuffer[88]{};

            static constexpr size_t ADDR_LEN = std::max(sizeof(coro::win::sockaddr_in), sizeof(coro::win::sockaddr_in6)) + 16;
            static_assert(sizeof(mBuffer) >= ADDR_LEN * 2);

            AcceptOperation(Socket &l, Socket &a) noexcept : mListeningSocket(l), mAcceptingSocket(a) {}
            static void cancelOperation(AcceptOperation &self) noexcept {
                CancelIoEx((sys::win::handle_t)self.mListeningSocket.getSocketHandle().get(), reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(&self)));
            }
            bool await_suspend(std::coroutine_handle<> h) noexcept {
                using LPFN_ACCEPTEX = sys::win::winbool_t(__stdcall *)(unsigned __int64 sListenSocket, unsigned __int64 sAcceptSocket, void *lpOutputBuffer, sys::win::dword_t dwReceiveDataLength, sys::win::dword_t dwLocalAddressLength, sys::win::dword_t dwRemoteAddressLength, sys::win::dword_t *lpdwBytesReceived, LPOVERLAPPED lpOverlapped);
                this->mContinuation = h;
                auto lpfnAcceptEx = getExFuncPtr<LPFN_ACCEPTEX>(
                    mListeningSocket.mHandle,
                    {
                        0xb5367df1, 0xcbac, 0x11cf, {0x95, 0xca, 0x00, 0x80, 0x5f, 0x48, 0xa1, 0x92}
                }
                );
                if (!lpfnAcceptEx) return false;
                sys::win::dword_t bytesReceived = 0;
                bool              ok = lpfnAcceptEx(
                    mListeningSocket.mHandle.get(),
                    mAcceptingSocket.mHandle.get(),
                    mBuffer,
                    0,
                    ADDR_LEN,
                    ADDR_LEN,
                    &bytesReceived,
                    reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(this))
                );
                return handleIoResult(ok);
            }

            NetResult<void> await_resume() noexcept {
                using LPFN_GETACCEPTEXSOCKADDRS = void(__stdcall *)(void *lpOutputBuffer, sys::win::dword_t dwReceiveDataLength, sys::win::dword_t dwLocalAddressLength, sys::win::dword_t dwRemoteAddressLength, struct sockaddr **LocalSockaddr, int *LocalSockaddrLength, struct sockaddr **RemoteSockaddr, int *RemoteSockaddrLength);
                if (mErrorCode != ERROR_SUCCESS) return {unexpected, static_cast<int>(mErrorCode), "AcceptEx"};
                auto lpfnGetAcceptExSockaddrs =
                    getExFuncPtr<LPFN_GETACCEPTEXSOCKADDRS>(
                        mAcceptingSocket.mHandle,
                        {
                            0xb5367df2, 0xcbac, 0x11cf, {0x95, 0xca, 0x00, 0x80, 0x5f, 0x48, 0xa1, 0x92}
                }
                    );
                if (mErrorCode != ERROR_SUCCESS)
                    return {unexpected, static_cast<int>(mErrorCode), "getExFuncPtr<LPFN_GETACCEPTEXSOCKADDRS>"};
                coro::win::sockaddr *localSockaddr = nullptr;
                coro::win::sockaddr *remoteSockaddr = nullptr;
                int                  localSockaddrLength;
                int                  remoteSockaddrLength;
                lpfnGetAcceptExSockaddrs(
                    mBuffer,
                    0,
                    ADDR_LEN,
                    ADDR_LEN,
                    (sockaddr **)&localSockaddr,
                    &localSockaddrLength,
                    (sockaddr **)&remoteSockaddr,
                    &remoteSockaddrLength
                );
                mAcceptingSocket.mLocalEndPoint = IPEndpoint::fromSockAddr(*localSockaddr);
                mAcceptingSocket.mRemoteEndPoint = IPEndpoint::fromSockAddr(*remoteSockaddr);
                win::socket_t socket = mListeningSocket.mHandle.get();
                int           result = setsockopt(
                    mAcceptingSocket.mHandle.get(),
                    0xffff,
                    0x700B, // SO_UPDATE_ACCEPT_CONTEXT
                    (const char *)&socket,
                    sizeof(win::socket_t)
                );
                if (result == -1)
                    return {unexpected, WSAGetLastError(), "setsockopt(SO_UPDATE_ACCEPT_CONTEXT)"};

                return {};
            }
        };

        auto accept(Socket &acceptingSocket) noexcept { return AcceptOperation{*this, acceptingSocket}; }

        auto accept(Socket &acceptingSocket, std::stop_token tk) noexcept {
            return CancelableOperation<AcceptOperation>{std::move(tk), *this, acceptingSocket};
        }

        struct [[nodiscard]] ConnectOperation : NetOperation {
            Socket    &mSocket;
            IPEndpoint mRemoteEp;

            ConnectOperation(Socket &s, IPEndpoint remoteEp) noexcept : mSocket(s), mRemoteEp(std::move(remoteEp)) {}
            static void cancelOperation(ConnectOperation &self) noexcept {
                CancelIoEx((sys::win::handle_t)self.mSocket.getSocketHandle().get(), reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(&self)));
            }
            bool await_suspend(std::coroutine_handle<> h) noexcept {
                using LPFN_CONNECTEX = sys::win::winbool_t(__stdcall *)(unsigned __int64 s, const struct sockaddr *name, int namelen, void *lpSendBuffer, sys::win::dword_t dwSendDataLength, sys::win::dword_t *lpdwBytesSent, LPOVERLAPPED lpOverlapped);
                this->mContinuation = h;
                auto lpfnConnectEx = getExFuncPtr<LPFN_CONNECTEX>(
                    mSocket.mHandle,
                    {
                        0x25a207b9, 0xddf3, 0x4660, {0x8e, 0xe9, 0x76, 0xe5, 0x8c, 0x74, 0x06, 0x3e}
                }
                );
                if (!lpfnConnectEx) return false;

                coro::win::sockaddr_storage remoteSockaddrStorage;
                coro::win::sockaddr        &sockaddr = reinterpret_cast<coro::win::sockaddr &>(remoteSockaddrStorage);
                int                         addrLength = mRemoteEp.toSockAddr(sockaddr);
                sys::win::dword_t           bytesSent = 0;
                bool                        ok = lpfnConnectEx(mSocket.mHandle.get(), (const struct sockaddr *)&sockaddr, addrLength, nullptr, 0, &bytesSent, reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(this)));
                return handleIoResult(ok);
            }
            NetResult<void> await_resume() noexcept {
                if (mErrorCode != ERROR_SUCCESS) { return {unexpected, static_cast<int>(mErrorCode), "ConnectEx"}; }
                int result = setsockopt(mSocket.mHandle.get(), 0xffff, 0x7010, nullptr, 0);
                if (result == -1) {
                    return {unexpected, WSAGetLastError(), "setsockopt(SO_UPDATE_CONNECT_CONTEXT)"};
                }
                return {};
            }
        };

        auto connect(const IPEndpoint &remoteEp) noexcept { return ConnectOperation{*this, remoteEp}; }
        auto connect(const IPEndpoint &remoteEp, std::uint16_t port) noexcept {
            IPEndpoint newPort = remoteEp;
            newPort.setPort(port);
            return ConnectOperation{*this, std::move(newPort)};
        }

        auto connect(const IPEndpoint &remoteEp, std::stop_token tk) noexcept {
            return CancelableOperation<ConnectOperation>{std::move(tk), *this, remoteEp};
        }
        auto connect(const IPEndpoint &remoteEp, std::uint16_t port, std::stop_token tk) noexcept {
            IPEndpoint newPort = remoteEp;
            newPort.setPort(port);
            return CancelableOperation<ConnectOperation>{std::move(tk), *this, std::move(newPort)};
        }

        struct [[nodiscard]] RecvOperation : NetOperation {
            Socket           &mSocket;
            coro::win::WSABuf mBuffer;

            RecvOperation(Socket &s, char *buffer, std::size_t size) noexcept :
                mSocket(s), mBuffer((sys::win::ulong_t)size, buffer) {}
            static void cancelOperation(RecvOperation &self) noexcept {
                CancelIoEx((sys::win::handle_t)self.mSocket.getSocketHandle().get(), reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(&self)));
            }
            bool await_suspend(std::coroutine_handle<> h) noexcept {
                this->mContinuation = h;
                sys::win::dword_t flags = 0;

                int result = WSARecv(mSocket.mHandle.get(), (LPWSABUF)&mBuffer, 1, &mBytesTransferred, &flags, reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(this)), nullptr);
                return handleIoResult(result);
            }
            NetResult<std::size_t> await_resume() noexcept {
                if (mErrorCode != ERROR_SUCCESS) return {unexpected, (int)mErrorCode};
                return mBytesTransferred;
            }
        };

        auto recv(void *buffer, std::size_t size) noexcept {
            return RecvOperation{*this, static_cast<char *>(buffer), size};
        }
        template <typename T>
            requires isConvertibleToCharSpan<T, true>
        auto recv(T &&buffer) noexcept {
            return recv(std::ranges::data(buffer), std::ranges::size(buffer));
        }

        auto recv(void *buffer, std::size_t size, std::stop_token tk) noexcept {
            return CancelableOperation<RecvOperation>{std::move(tk), *this, static_cast<char *>(buffer), size};
        }
        template <typename T>
            requires isConvertibleToCharSpan<T, true>
        auto recv(T &&buffer, std::stop_token tk) noexcept {
            return recv(std::ranges::data(buffer), std::ranges::size(buffer), std::move(tk));
        }

        struct RecvFromResult {
            std::size_t mBytesTransferred;
            IPEndpoint  mEndPoint;
        };

        struct [[nodiscard]] RecvFromOperation : NetOperation {
            Socket           &mSocket;
            coro::win::WSABuf mBuffer;
            alignas(4) std::uint8_t mSourceSockaddrStorage[28];
            int mSourceSockaddrLength;

            RecvFromOperation(Socket &s, char *buffer, std::size_t size) noexcept :
                mSocket(s), mBuffer((sys::win::ulong_t)size, buffer) {}
            static void cancelOperation(RecvFromOperation &self) noexcept {
                CancelIoEx((sys::win::handle_t)self.mSocket.getSocketHandle().get(), reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(&self)));
            }
            bool await_suspend(std::coroutine_handle<> h) noexcept {
                this->mContinuation = h;
                sys::win::dword_t flags = 0;
                mSourceSockaddrLength = sizeof(mSourceSockaddrStorage);
                int result = WSARecvFrom(
                    mSocket.mHandle.get(),
                    (LPWSABUF)&mBuffer,
                    1,
                    &mBytesTransferred,
                    &flags,
                    reinterpret_cast<struct sockaddr *>(&mSourceSockaddrStorage),
                    &mSourceSockaddrLength,
                    reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(this)),
                    nullptr
                );
                return handleIoResult(result);
            }
            NetResult<RecvFromResult> await_resume() noexcept {
                if (mErrorCode != ERROR_SUCCESS) return {unexpected, (int)mErrorCode};
                return {
                    inPlace,
                    mBytesTransferred,
                    IPEndpoint::fromSockAddr(*std::launder(reinterpret_cast<coro::win::sockaddr *>(&mSourceSockaddrStorage)))
                };
            }
        };

        auto recvFrom(void *buffer, std::size_t size) noexcept {
            return RecvFromOperation{*this, static_cast<char *>(buffer), size};
        }
        template <typename T>
            requires isConvertibleToCharSpan<T, true>
        auto recvFrom(T &&buffer) noexcept {
            return recvFrom(std::ranges::data(buffer), std::ranges::size(buffer));
        }

        auto recvFrom(void *buffer, std::size_t size, std::stop_token tk) noexcept {
            return CancelableOperation<RecvFromOperation>{
                std::move(tk), *this, static_cast<char *>(buffer), size
            };
        }
        template <typename T>
            requires isConvertibleToCharSpan<T, true>
        auto recvFrom(T &&buffer, std::stop_token tk) noexcept {
            return recvFrom(std::ranges::data(buffer), std::ranges::size(buffer), std::move(tk));
        }

        struct [[nodiscard]] SendOperation : NetOperation {
            Socket           &mSocket;
            coro::win::WSABuf mBuffer;

            SendOperation(Socket &s, char *buffer, std::size_t size) noexcept :
                mSocket(s), mBuffer((sys::win::ulong_t)size, buffer) {}
            static void cancelOperation(SendOperation &self) noexcept {
                CancelIoEx((sys::win::handle_t)self.mSocket.getSocketHandle().get(), reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(&self)));
            }
            bool await_suspend(std::coroutine_handle<> h) noexcept {
                this->mContinuation = h;
                int result = WSASend(mSocket.mHandle.get(), (LPWSABUF)&mBuffer, 1, &mBytesTransferred, 0, reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(this)), nullptr);
                return handleIoResult(result);
            }
            NetResult<std::size_t> await_resume() noexcept {
                if (mErrorCode != ERROR_SUCCESS) return {unexpected, (int)mErrorCode};
                return mBytesTransferred;
            }
        };

        auto send(const void *buffer, std::size_t size) noexcept {
            return SendOperation{*this, static_cast<char *>(const_cast<void *>(buffer)), size};
        }
        template <typename T>
            requires isConvertibleToCharSpan<T, true>
        auto send(T &&buffer) noexcept {
            return send(std::ranges::data(buffer), std::ranges::size(buffer));
        }

        auto send(const void *buffer, std::size_t size, std::stop_token tk) noexcept {
            return CancelableOperation<SendOperation>{
                std::move(tk), *this, static_cast<char *>(const_cast<void *>(buffer)), size
            };
        }
        template <typename T>
            requires isConvertibleToCharSpan<T, true>
        auto send(T &&buffer, std::stop_token tk) noexcept {
            return send(std::ranges::data(buffer), std::ranges::size(buffer), std::move(tk));
        }

        struct [[nodiscard]] SendToOperation : NetOperation {
            Socket           &mSocket;
            IPEndpoint        mDest;
            coro::win::WSABuf mBuffer;

            SendToOperation(Socket &s, const IPEndpoint &dest, char *buffer, std::size_t size) noexcept :
                mSocket(s), mDest(dest), mBuffer((sys::win::ulong_t)size, buffer) {}
            static void cancelOperation(SendToOperation &self) noexcept {
                CancelIoEx((sys::win::handle_t)self.mSocket.getSocketHandle().get(), reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(&self)));
            }
            bool await_suspend(std::coroutine_handle<> h) noexcept {
                this->mContinuation = h;
                coro::win::sockaddr_storage addrStorage;
                coro::win::sockaddr        &sockaddr = reinterpret_cast<coro::win::sockaddr &>(addrStorage);
                const int                   addrLength = mDest.toSockAddr(sockaddr);
                int                         result = WSASendTo(
                    mSocket.mHandle.get(), (LPWSABUF)&mBuffer, 1, &mBytesTransferred, 0, (const struct sockaddr *)&sockaddr, addrLength, reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(this)), nullptr
                );
                return handleIoResult(result);
            }
            NetResult<std::size_t> await_resume() noexcept {
                if (mErrorCode != ERROR_SUCCESS) return {unexpected, (int)mErrorCode};
                return mBytesTransferred;
            }
        };

        auto sendTo(const IPEndpoint &dest, const void *buffer, std::size_t size) noexcept {
            return SendToOperation{*this, dest, static_cast<char *>(const_cast<void *>(buffer)), size};
        }
        template <typename T>
            requires isConvertibleToCharSpan<T, true>
        auto sendTo(const IPEndpoint &dest, T &&buffer) noexcept {
            return sendTo(dest, std::ranges::data(buffer), std::ranges::size(buffer));
        }

        auto sendTo(const IPEndpoint &dest, const void *buffer, std::size_t size, std::stop_token tk) noexcept {
            return CancelableOperation<SendToOperation>{
                std::move(tk), *this, dest, static_cast<char *>(const_cast<void *>(buffer)), size
            };
        }
        template <typename T>
            requires isConvertibleToCharSpan<T, true>
        auto sendTo(const IPEndpoint &dest, T &&buffer, std::stop_token tk) noexcept {
            return sendTo(dest, std::ranges::data(buffer), std::ranges::size(buffer), std::move(tk));
        }

        struct [[nodiscard]] SendVOperation : NetOperation {
            Socket                        &mSocket;
            std::vector<coro::win::WSABuf> mBuffers;

            template <typename... Args>
            SendVOperation(Socket &s, Args &&...buffers) noexcept : mSocket(s), mBuffers(sizeof...(buffers)) {
                auto _ = {
                    (mBuffers.emplace_back((sys::win::ulong_t)std::ranges::size(buffers), (char *)std::ranges::data(buffers)), 0)...
                };
            }
            static void cancelOperation(SendVOperation &self) noexcept {
                CancelIoEx((sys::win::handle_t)self.mSocket.getSocketHandle().get(), reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(&self)));
            }
            bool await_suspend(std::coroutine_handle<> h) noexcept {
                this->mContinuation = h;
                int result = WSASend(
                    mSocket.mHandle.get(), (LPWSABUF)mBuffers.data(), (sys::win::dword_t)mBuffers.size(), &mBytesTransferred, 0, reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(this)), nullptr
                );
                return handleIoResult(result);
            }
            NetResult<std::size_t> await_resume() noexcept {
                if (mErrorCode != ERROR_SUCCESS) return {unexpected, (int)mErrorCode};
                return mBytesTransferred;
            }
        };

        template <typename... Args>
            requires(sizeof...(Args) > 1 && (isConvertibleToCharSpan<Args, true> && ...))
        auto sendv(Args &&...args) noexcept {
            return SendVOperation{*this, std::forward<Args>(args)...};
        }
        template <typename... Args>
            requires(sizeof...(Args) > 1 && (isConvertibleToCharSpan<Args, true> && ...))
        auto sendv(std::stop_token tk, Args &&...args) noexcept {
            return CancelableOperation<SendVOperation>{std::move(tk), *this, std::forward<Args>(args)...};
        }
    };

} // namespace sapphire::coro::net
