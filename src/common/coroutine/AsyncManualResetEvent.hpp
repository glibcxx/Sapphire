#pragma once

#include <atomic>
#include <coroutine>

namespace sapphire::coro {

    class AsyncManualResetEvent {
        static constexpr uintptr_t SET_NO_WAITERS = 0;

        /*
            - this: 已 set
            - SET_NO_WAITERS: 未 set，无等待者
            - 其它指针: 未 set，等待队列的头指针
        */
        std::atomic<uintptr_t> mState;

        struct [[nodiscard]] AsyncManualResetEventAwaiter {
            AsyncManualResetEvent        &mEvent;
            AsyncManualResetEventAwaiter *mNext;
            std::coroutine_handle<>       mAwaiter;

            explicit AsyncManualResetEventAwaiter(AsyncManualResetEvent &e) noexcept : mEvent(e) {}

            bool await_ready() const noexcept { return mEvent.isSet(); }
            bool await_suspend(std::coroutine_handle<> awaiter) noexcept {
                mAwaiter = awaiter;
                const uintptr_t setState = reinterpret_cast<uintptr_t>(&mEvent);

                uintptr_t oldState = mEvent.mState.load(std::memory_order_acquire);
                do {
                    if (oldState == setState) { return false; }

                    mNext = reinterpret_cast<AsyncManualResetEventAwaiter *>(oldState);
                } while (!mEvent.mState.compare_exchange_weak(
                    oldState, reinterpret_cast<uintptr_t>(this), std::memory_order_release, std::memory_order_acquire
                ));

                return true;
            }
            void await_resume() const noexcept {}
        };

    public:
        constexpr AsyncManualResetEvent() = default;

        AsyncManualResetEvent(const AsyncManualResetEvent &) = delete;
        AsyncManualResetEvent &operator=(const AsyncManualResetEvent &) = delete;

        void set() noexcept {
            const uintptr_t setState = reinterpret_cast<uintptr_t>(this);
            uintptr_t       oldState = mState.exchange(setState, std::memory_order_acq_rel);
            if (oldState != setState) {
                auto *current = reinterpret_cast<AsyncManualResetEventAwaiter *>(oldState);
                while (current != nullptr) {
                    auto *next = current->mNext;
                    current->mAwaiter.resume();
                    current = next;
                }
            }
        }

        bool isSet() const noexcept {
            return mState.load(std::memory_order_acquire) == reinterpret_cast<uintptr_t>(this);
        }

        void reset() noexcept {
            uintptr_t oldState = reinterpret_cast<uintptr_t>(this);
            mState.compare_exchange_strong(oldState, SET_NO_WAITERS, std::memory_order_relaxed);
        }

        AsyncManualResetEventAwaiter operator co_await() { return AsyncManualResetEventAwaiter{*this}; }
    };

} // namespace sapphire::coro