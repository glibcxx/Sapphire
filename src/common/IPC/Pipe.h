#pragma once

#include <string>
#include <Windows.h>
#include <sddl.h>
#include <system_error>
#include <utility>
#include <winerror.h>

#include "common/Expected.hpp"
#include "common/coroutine/FileOperation.hpp"
#include "common/coroutine/IoContext.hpp"

namespace sapphire::ipc::backend {

    class Pipe {
        HANDLE mHandle = INVALID_HANDLE_VALUE;

        Pipe(HANDLE hPipe) : mHandle(hPipe) {}

        static sys::win::handle_t getEventHandleForThisThread() noexcept {
            static thread_local coro::win32::Handle tEvent = CreateEventW(NULL, FALSE, FALSE, NULL);
            return tEvent.get();
        }

    public:
        ~Pipe() noexcept { disconnect(); }

        Pipe(const Pipe &) = delete;
        Pipe &operator=(const Pipe &) = delete;

        Pipe(Pipe &&other) noexcept : mHandle(std::exchange(other.mHandle, INVALID_HANDLE_VALUE)) {}
        Pipe &operator=(Pipe &&other) noexcept {
            if (this != &other) {
                mHandle = std::exchange(other.mHandle, INVALID_HANDLE_VALUE);
            }
            return *this;
        }

        bool isOpen() const noexcept { return mHandle && mHandle != INVALID_HANDLE_VALUE; }

        [[nodiscard]] static Expected<Pipe, std::error_code> connect(const std::wstring &pipeName, coro::IoContext &ctx, int timeout = 20000) noexcept {
            while (true) {
                HANDLE hPipe = CreateFileW(
                    pipeName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr
                );

                if (hPipe != INVALID_HANDLE_VALUE) {
                    Pipe res{hPipe};

                    if (auto errc = ctx.attach(hPipe))
                        return {unexpected, std::move(errc)};

                    if (!SetFileCompletionNotificationModes(hPipe, FILE_SKIP_COMPLETION_PORT_ON_SUCCESS))
                        return {unexpected, GetLastError(), std::system_category()};

                    return res;
                }

                int code = GetLastError();
                if (code != ERROR_PIPE_BUSY) {
                    return {unexpected, code, std::system_category()};
                }

                if (!WaitNamedPipeW(pipeName.c_str(), timeout)) {
                    return {unexpected, GetLastError(), std::system_category()};
                }
            }
        }

        [[nodiscard]] static Expected<Pipe, std::error_code> create(const std::wstring &pipeName, coro::IoContext &ctx) noexcept {
            PSECURITY_DESCRIPTOR p_sd = nullptr;
            SECURITY_ATTRIBUTES  sa = {};
            sa.nLength = sizeof(SECURITY_ATTRIBUTES);
            sa.bInheritHandle = FALSE;
            ConvertStringSecurityDescriptorToSecurityDescriptorW(
                L"D:(A;;GA;;;WD)(A;;GA;;;AC)",
                SDDL_REVISION_1,
                &p_sd,
                nullptr
            );
            sa.lpSecurityDescriptor = p_sd;
            if (p_sd) LocalFree(p_sd);

            HANDLE hPipe = CreateNamedPipeW(
                pipeName.c_str(),
                PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
                PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
                PIPE_UNLIMITED_INSTANCES,
                4096,
                4096,
                0,
                &sa
            );
            if (hPipe == INVALID_HANDLE_VALUE) {
                return {unexpected, GetLastError(), std::system_category()};
            }

            Pipe res{hPipe};

            if (auto errc = ctx.attach(hPipe))
                return {unexpected, std::move(errc)};

            if (!SetFileCompletionNotificationModes(hPipe, FILE_SKIP_COMPLETION_PORT_ON_SUCCESS))
                return {unexpected, GetLastError(), std::system_category()};

            return res;
        }

        struct [[nodiscard]] PipeListenOperation : coro::IoOperation {
            using IoOperation::IoOperation;

            sys::win::handle_t mFileHandle;

            PipeListenOperation(sys::win::handle_t h) noexcept : mFileHandle(h) {}

            bool await_ready() const noexcept { return false; }

            bool await_suspend(std::coroutine_handle<> h) noexcept {
                this->mContinuation = h;
                BOOL ok = ConnectNamedPipe(mFileHandle, reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(this)));
                return handleIoResult(ok);
            }

            std::error_code await_resume() noexcept {
                if (mErrorCode != ERROR_SUCCESS && mErrorCode != ERROR_PIPE_CONNECTED) {
                    return {static_cast<int>(mErrorCode), std::system_category()};
                }
                return {};
            }

            bool handleIoResult(bool ok) noexcept {
                if (ok == 1) return false;
                int err = GetLastError();
                if (err == ERROR_IO_PENDING) return true;
                mErrorCode = err;
                return false;
            }
        };

        PipeListenOperation listen() noexcept {
            return PipeListenOperation{mHandle};
        }

        Expected<size_t, std::error_code> listenSync() noexcept {
            auto hEvent = getEventHandleForThisThread();

            OVERLAPPED ov = {0};
            ov.hEvent = (HANDLE)((ULONG_PTR)hEvent | 1);

            DWORD bytesTransferred = 0;
            BOOL  result = ConnectNamedPipe(mHandle, &ov);
            if (!result) {
                if (GetLastError() == ERROR_IO_PENDING) {
                    WaitForSingleObject(hEvent, INFINITE);
                    GetOverlappedResult(mHandle, &ov, &bytesTransferred, FALSE);
                } else {
                    return {unexpected, GetLastError(), std::system_category()};
                }
            }
            return bytesTransferred;
        }

        void disconnect() noexcept {
            DisconnectNamedPipe(mHandle);
            CloseHandle(mHandle);
            mHandle = INVALID_HANDLE_VALUE;
        }

        struct [[nodiscard]] PipeReadOperation : coro::FileOperation {
            using FileOperation::FileOperation;

            bool await_suspend(std::coroutine_handle<> h) noexcept {
                this->mContinuation = h;
                bool ok = ReadFile(mFileHandle, mBuffer, mBufferSize, &mBytesTransferred, reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(this)));
                return handleIoResult(ok);
            }

            Expected<size_t, std::error_code> await_resume() noexcept {
                if (mErrorCode != ERROR_SUCCESS) {
                    return {unexpected, static_cast<int>(mErrorCode), std::system_category()};
                }
                return mBytesTransferred;
            }
        };

        PipeReadOperation read(char *buffer, std::size_t bufferSize) noexcept {
            return PipeReadOperation{mHandle, 0, buffer, bufferSize};
        }

        Expected<size_t, std::error_code> readSync(char *buffer, std::size_t bufferSize) noexcept {
            auto hEvent = getEventHandleForThisThread();

            OVERLAPPED ov = {0};
            ov.hEvent = (HANDLE)((ULONG_PTR)hEvent | 1);

            DWORD bytesTransferred = 0;
            BOOL  result = ReadFile(mHandle, buffer, bufferSize, &bytesTransferred, &ov);
            if (!result) {
                if (GetLastError() == ERROR_IO_PENDING) {
                    WaitForSingleObject(hEvent, INFINITE);
                    GetOverlappedResult(mHandle, &ov, &bytesTransferred, FALSE);
                } else {
                    return {unexpected, GetLastError(), std::system_category()};
                }
            }
            return bytesTransferred;
        }

        struct [[nodiscard]] PipeWriteOperation : coro::FileOperation {
            using FileOperation::FileOperation;

            bool await_suspend(std::coroutine_handle<> h) noexcept {
                this->mContinuation = h;
                bool ok = WriteFile(mFileHandle, mBuffer, mBufferSize, &mBytesTransferred, reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(this)));
                return handleIoResult(ok);
            }
        };

        PipeWriteOperation write(const char *buffer, std::size_t bufferSize) noexcept {
            return PipeWriteOperation{mHandle, 0, const_cast<char *>(buffer), bufferSize};
        }

        Expected<size_t, std::error_code> writeSync(const char *buffer, std::size_t bufferSize) noexcept {
            auto hEvent = getEventHandleForThisThread();

            OVERLAPPED ov = {0};
            ov.hEvent = (HANDLE)((ULONG_PTR)hEvent | 1);

            DWORD bytesTransferred = 0;
            BOOL  result = WriteFile(mHandle, buffer, bufferSize, &bytesTransferred, &ov);
            if (!result) {
                if (GetLastError() == ERROR_IO_PENDING) {
                    WaitForSingleObject(hEvent, INFINITE);
                    GetOverlappedResult(mHandle, &ov, &bytesTransferred, FALSE);
                } else {
                    return {unexpected, GetLastError(), std::system_category()};
                }
            }
            return bytesTransferred;
        }
    };

} // namespace sapphire::ipc::backend