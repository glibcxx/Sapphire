#pragma once

#include <atomic>
#include <coroutine>
#include <exception>
#include "Awaitable.hpp"

namespace sapphire::coro {

    class AsyncScope {
        std::atomic<size_t>     mCount;
        std::coroutine_handle<> mContinuation;

        struct AsyncScopeTask {
            struct promise_type {
                AsyncScope *mScope;

                std::suspend_always initial_suspend() const noexcept { return {}; }
                auto                final_suspend() const noexcept {
                    struct OnExitAwaiter {
                        AsyncScope *mScope;
                        bool        await_ready() const noexcept {
                            return mScope->mCount.fetch_sub(1, std::memory_order_relaxed) != 1;
                        }
                        bool await_suspend(std::coroutine_handle<>) const noexcept {
                            mScope->mContinuation.resume();
                            return false;
                        }
                        constexpr void await_resume() const noexcept {}
                    };
                    return OnExitAwaiter{mScope};
                }
                [[noreturn]] void unhandled_exception() const noexcept { std::terminate(); }
                AsyncScopeTask    get_return_object() {
                    return {std::coroutine_handle<promise_type>::from_promise(*this)};
                }
                constexpr void return_void() const {}
            };
            using CoroHandle = std::coroutine_handle<promise_type>;

            CoroHandle mHandle;
        };

    public:
        AsyncScope() noexcept : mCount(1), mContinuation(nullptr) {}

        template <IsAwaitable Awaitable>
        void spawn(Awaitable &&awaitable) {
            mCount.fetch_add(1, std::memory_order_relaxed);
            AsyncScopeTask task = [](Awaitable awaitable) -> AsyncScopeTask {
                co_await std::forward<Awaitable>(awaitable);
            }(std::forward<Awaitable>(awaitable));
            task.mHandle.promise().mScope = this;
            task.mHandle.resume();
        }

        struct [[nodiscard]] JoinAwaiter {
            AsyncScope *mScope;

            bool await_ready() const noexcept { return mScope->mCount.load(std::memory_order_acquire) == 0; }
            bool await_suspend(std::coroutine_handle<> callerHandle) const noexcept {
                mScope->mContinuation = callerHandle;
                return mScope->mCount.fetch_sub(1u, std::memory_order_acq_rel) > 1u;
            }
            constexpr void await_resume() const noexcept {}
        };

        JoinAwaiter join() noexcept { return JoinAwaiter{this}; }
    };

} // namespace sapphire::coro