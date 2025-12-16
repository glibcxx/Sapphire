#pragma once

namespace sapphire::coro {

    struct AsyncScopedLock;

    class AsyncMutex {
        static constexpr uintptr_t NOT_LOCKED = 0;
        static constexpr uintptr_t LOCKED_NO_WAITERS = 1;

        struct AsyncLockAwaiter {
            AsyncMutex             &mMutex;
            std::coroutine_handle<> mAwaitingCoroHandle;
            AsyncLockAwaiter       *mNext = nullptr;

            constexpr explicit AsyncLockAwaiter(AsyncMutex &mutex) : mMutex(mutex) {}

            bool await_ready() noexcept {
                auto expected = AsyncMutex::NOT_LOCKED;
                if (mMutex.mState.compare_exchange_strong(
                        expected,
                        AsyncMutex::LOCKED_NO_WAITERS,
                        std::memory_order_acquire
                    )) {
                    return true;
                }
                return false;
            }

            bool await_suspend(std::coroutine_handle<> handle) noexcept {
                mAwaitingCoroHandle = handle;
                uintptr_t oldState = mMutex.mState.load(std::memory_order_relaxed);
                while (true) {
                    if (oldState == AsyncMutex::NOT_LOCKED) {
                        if (mMutex.mState.compare_exchange_weak(
                                oldState,
                                AsyncMutex::LOCKED_NO_WAITERS,
                                std::memory_order_acquire,
                                std::memory_order_relaxed
                            )) {
                            return false;
                        }
                    } else {
                        mNext = reinterpret_cast<AsyncLockAwaiter *>(oldState == AsyncMutex::LOCKED_NO_WAITERS ? 0 : oldState);
                        if (mMutex.mState.compare_exchange_weak(
                                oldState,
                                reinterpret_cast<uintptr_t>(this),
                                std::memory_order_release,
                                std::memory_order_relaxed
                            )) {
                            return true;
                        }
                    }
                }
            }

            [[nodiscard]] AsyncScopedLock await_resume() noexcept;
        };

        /*
            - NOT_LOCKED: 未加锁
            - LOCKED_NO_WAITERS: 已加锁，无等待者
            - 其它指针: 已加锁，等待队列的头指针
        */
        std::atomic<uintptr_t> mState = NOT_LOCKED;
        AsyncLockAwaiter      *mWaiters = nullptr;

    public:
        constexpr AsyncMutex() = default;

        AsyncMutex(const AsyncMutex &) = delete;
        AsyncMutex &operator=(const AsyncMutex &) = delete;

        [[nodiscard]] constexpr auto lockAsync() {
            return AsyncLockAwaiter{*this};
        }

        void unlock() noexcept {
            AsyncLockAwaiter *waitersHead = mWaiters;
            if (waitersHead == nullptr) {
                auto       oldState = LOCKED_NO_WAITERS;
                const bool releasedLock = mState.compare_exchange_strong(
                    oldState,
                    NOT_LOCKED,
                    std::memory_order_release,
                    std::memory_order_relaxed
                );
                if (releasedLock)
                    return;

                oldState = mState.exchange(LOCKED_NO_WAITERS, std::memory_order_acquire);
                auto next = reinterpret_cast<AsyncLockAwaiter *>(oldState);
                do {
                    auto *temp = next->mNext;
                    next->mNext = waitersHead;
                    waitersHead = next;
                    next = temp;
                } while (next != nullptr);
            }
            mWaiters = waitersHead->mNext;
            waitersHead->mAwaitingCoroHandle.resume();
        }
    };

    struct AsyncScopedLock {
        AsyncMutex &mMutex;

        constexpr AsyncScopedLock(AsyncMutex &mutex) noexcept : mMutex(mutex) {}
        ~AsyncScopedLock() noexcept { mMutex.unlock(); }

        AsyncScopedLock &operator=(AsyncScopedLock &&) = delete;
    };

    inline AsyncScopedLock AsyncMutex::AsyncLockAwaiter::await_resume() noexcept {
        return AsyncScopedLock{this->mMutex};
    }

} // namespace sapphire::coro