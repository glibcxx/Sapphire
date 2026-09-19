#pragma once

#include <coroutine>
#include <atomic>
#include <cstddef>
#include <mutex>
#include <cassert>
#include "SpinMutex.hpp"

namespace sapphire::coro {

    class AsyncSemaphore {
    public:
        explicit AsyncSemaphore(std::ptrdiff_t initialCount = 1) noexcept : mCount(initialCount) {
            assert(initialCount >= 0);
        }

        ~AsyncSemaphore() noexcept { assert(mHead == nullptr && "Destructing semaphore with active waiters!"); }

        AsyncSemaphore(const AsyncSemaphore &) = delete;
        AsyncSemaphore &operator=(const AsyncSemaphore &) = delete;

        bool tryAcquire() noexcept {
            auto old_count = mCount.load(std::memory_order_relaxed);
            while (old_count > 0) {
                if (mCount.compare_exchange_weak(
                        old_count, old_count - 1, std::memory_order_acquire, std::memory_order_relaxed
                    )) {
                    return true;
                }
            }
            return false;
        }

        struct [[nodiscard]] AsyncScopedSemaphore {
            constexpr AsyncScopedSemaphore(AsyncSemaphore &sem) noexcept : mSem(sem) {}
            ~AsyncScopedSemaphore() noexcept { mSem.release(); }

            AsyncScopedSemaphore &operator=(AsyncScopedSemaphore &&) = delete;

        private:
            AsyncSemaphore &mSem;
        };

        struct [[nodiscard]] AcquireAwaiter {
            AsyncSemaphore         &mSem;
            AcquireAwaiter         *mNext{nullptr};
            std::coroutine_handle<> mHandle{nullptr};

            bool await_ready() const noexcept {
                auto cur = mSem.mCount.load(std::memory_order_relaxed);
                while (cur > 0) {
                    if (mSem.mCount.compare_exchange_weak(
                            cur, cur - 1, std::memory_order_acquire, std::memory_order_relaxed
                        )) {
                        return true;
                    }
                }
                return false;
            }

            bool await_suspend(std::coroutine_handle<> h) noexcept {
                std::lock_guard lock{mSem.mMutex};
                if (mSem.mCount.load(std::memory_order_relaxed) > 0) {
                    mSem.mCount.fetch_sub(1, std::memory_order_relaxed);
                    return false;
                }

                mHandle = h;
                mNext = nullptr;
                if (!mSem.mTail) {
                    mSem.mHead = mSem.mTail = this;
                } else {
                    mSem.mTail->mNext = this;
                    mSem.mTail = this;
                }
                return true;
            }
            AsyncScopedSemaphore await_resume() noexcept { return AsyncScopedSemaphore{mSem}; }
        };

        AcquireAwaiter acquire() noexcept { return AcquireAwaiter{*this}; }

        void release(std::ptrdiff_t update = 1) noexcept {
            assert(update > 0);
            AcquireAwaiter *toResumeHead = nullptr;
            AcquireAwaiter *toResumeTail = nullptr;
            {
                std::lock_guard lock{mMutex};
                while (update > 0 && mHead != nullptr) {
                    AcquireAwaiter *w = mHead;
                    mHead = mHead->mNext;
                    if (!mHead) mTail = nullptr;
                    w->mNext = nullptr;

                    if (!toResumeHead) {
                        toResumeHead = toResumeTail = w;
                    } else {
                        toResumeTail->mNext = w;
                        toResumeTail = w;
                    }
                    --update;
                }
                if (update > 0) mCount.fetch_add(update, std::memory_order_release);
            }
            while (toResumeHead) {
                AcquireAwaiter *next = toResumeHead->mNext;
                toResumeHead->mHandle.resume();
                toResumeHead = next;
            }
        }

    private:
        std::atomic<std::ptrdiff_t> mCount;
        SpinMutex                   mMutex;
        AcquireAwaiter             *mHead{nullptr};
        AcquireAwaiter             *mTail{nullptr};
    };

} // namespace sapphire::coro