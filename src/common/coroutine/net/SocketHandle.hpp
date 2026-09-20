#pragma once

#include <cstdint>
#include <utility>

#include "../detail/WinDefs.h"

namespace sapphire::coro::net::win {

    using socket_t = std::uintptr_t;

    class SocketHandle {
        socket_t mHandle = INVALID_SOCKET_VALUE;

    public:
        constexpr static socket_t INVALID_SOCKET_VALUE = (socket_t)~0;

        SocketHandle() = default;
        SocketHandle(socket_t handle) noexcept : mHandle(handle) {}
        ~SocketHandle() { close(); }

        SocketHandle(const SocketHandle &) = delete;
        SocketHandle &operator=(const SocketHandle &) = delete;

        SocketHandle(SocketHandle &&other) noexcept : mHandle(std::exchange(other.mHandle, INVALID_SOCKET_VALUE)) {}
        SocketHandle &operator=(SocketHandle &&other) noexcept {
            if (this != &other) {
                close();
                mHandle = std::exchange(other.mHandle, INVALID_SOCKET_VALUE);
            }
            return *this;
        }
        SocketHandle &operator=(socket_t handle) noexcept {
            close();
            mHandle = std::exchange(handle, INVALID_SOCKET_VALUE);
            return *this;
        }

        void close() noexcept {
            if (mHandle != INVALID_SOCKET_VALUE) {
                closesocket(mHandle);
                mHandle = INVALID_SOCKET_VALUE;
            }
        }

        socket_t get() const noexcept { return mHandle; }
        bool     isValid() const noexcept { return mHandle != INVALID_SOCKET_VALUE; }
    };

} // namespace sapphire::net::win