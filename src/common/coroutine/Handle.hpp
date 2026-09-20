#pragma once

#include <utility>
#include "common/sys/MiniWindows.h"

namespace sapphire::coro::win32 {

    class Handle {
        uintptr_t mHandle = (uintptr_t)-1;

    public:
        static constexpr uintptr_t INVALID_HANDLE = (uintptr_t)-1;

        Handle() = default;
        Handle(sys::win::handle_t handle) noexcept : mHandle((uintptr_t)handle) {}
        ~Handle() noexcept { close(); }

        Handle(const Handle &) = delete;
        Handle &operator=(const Handle &) = delete;

        Handle(Handle &&other) noexcept : mHandle(std::exchange(other.mHandle, INVALID_HANDLE)) {}
        Handle &operator=(Handle &&other) noexcept {
            if (this != &other) {
                close();
                mHandle = std::exchange(other.mHandle, INVALID_HANDLE);
            }
            return *this;
        }
        Handle &operator=(sys::win::handle_t handle) noexcept {
            close();
            mHandle = (uintptr_t)handle;
            return *this;
        }

        void close() noexcept {
            if (mHandle != INVALID_HANDLE) {
                CloseHandle((sys::win::handle_t)mHandle);
                mHandle = INVALID_HANDLE;
            }
        }

        sys::win::handle_t get() const noexcept { return (sys::win::handle_t)mHandle; }
        bool               isValid() const noexcept { return mHandle != INVALID_HANDLE; }
    };

} // namespace sapphire::coro::win32