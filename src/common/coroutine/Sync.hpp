#pragma once

#include "common/sys/MiniWindows.h"

namespace sapphire::coro::detail {

    class mutex {
    public:
        using native_handle_type = void *;

        constexpr mutex() noexcept : mLock(nullptr) {}

        ~mutex() = default;

        mutex(const mutex &) = delete;
        mutex &operator=(const mutex &) = delete;

        void lock() noexcept { AcquireSRWLockExclusive((PSRWLOCK)&mLock); }

        bool try_lock() noexcept { return TryAcquireSRWLockExclusive((PSRWLOCK)&mLock) != 0; }

        void unlock() noexcept { ReleaseSRWLockExclusive((PSRWLOCK)&mLock); }

        native_handle_type native_handle() noexcept { return &mLock; }

    private:
        void *mLock;
    };

} // namespace sapphire::coro::detail