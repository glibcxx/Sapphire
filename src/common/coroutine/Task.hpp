#pragma once

#include <atomic>
#include <cassert>
#include <coroutine>
#include <exception>
#include <format>
#include <iostream>
#include <type_traits>
#include <variant>

namespace sapphire::coro {

    template <typename TResult>
    class Task;

    namespace detail {

        class TaskPromiseBase {
        public:
            constexpr std::suspend_always initial_suspend() const noexcept { return {}; }

            struct TaskFinalAwaiter {
                constexpr bool await_ready() const noexcept { return false; }
                template <typename TPromise>
                auto await_suspend(std::coroutine_handle<TPromise> h) const noexcept {
                    std::coroutine_handle<> continuation = h.promise().mContinuation;
                    return continuation ? continuation : std::noop_coroutine();
                }
                constexpr void await_resume() const noexcept {}
            };

            constexpr auto final_suspend() const noexcept {
                return TaskFinalAwaiter{};
            }

            std::coroutine_handle<> mContinuation{nullptr};
        };

        template <typename TResult>
        class TaskPromise : public TaskPromiseBase {
        public:
            using CoroHandleType = std::coroutine_handle<TaskPromise>;

            Task<TResult> get_return_object() noexcept;
            void          unhandled_exception() noexcept { mResult = std::current_exception(); }
            template <typename TValue>
                requires std::is_convertible_v<TValue &&, TResult>
            constexpr void return_value(TValue &&value) noexcept(std::is_nothrow_constructible_v<TResult, TValue &&>) {
                mResult.template emplace<1>(std::forward<TValue>(value));
            }

            TResult &result() & {
                if (mResult.index() == 2) std::rethrow_exception(std::get<2>(mResult));
                assert(mResult.index() == 1 && "Result value is not set. WHY??");
                return std::get<1>(mResult);
            }

            TResult &&result() && {
                if (mResult.index() == 2) std::rethrow_exception(std::get<2>(mResult));
                assert(mResult.index() == 1 && "Result value is not set. WHY??");
                return std::move(std::get<1>(mResult));
            }

            std::variant<std::monostate, TResult, std::exception_ptr> mResult;
        };

        template <>
        class TaskPromise<void> : public TaskPromiseBase {
        public:
            using CoroHandleType = std::coroutine_handle<TaskPromise<void>>;

            Task<void>     get_return_object() noexcept;
            void           unhandled_exception() noexcept { mResult = std::current_exception(); }
            constexpr void return_void() const noexcept {}

            void result() const {
                if (mResult) std::rethrow_exception(mResult);
            }

            std::exception_ptr mResult;
        };

        template <typename TResult>
        class TaskPromise<TResult &> : public TaskPromiseBase {
        public:
            using CoroHandleType = std::coroutine_handle<TaskPromise<TResult &>>;

            Task<TResult &> get_return_object() noexcept;
            void            unhandled_exception() noexcept { mResult = std::current_exception(); }
            constexpr void  return_value(TResult &value) noexcept {
                mResult.template emplace<1>(std::addressof(value));
            }

            TResult &result() {
                if (mResult.index() == 2) std::rethrow_exception(std::get<2>(mResult));
                return *std::get<1>(mResult);
            }

            std::variant<std::monostate, TResult *, std::exception_ptr> mResult;
        };

    } // namespace detail

    template <typename TResult>
    class Task<TResult &&> {
        static_assert(false, "Rvalue reference is invalid. PLEASE use Task<TResult>.");
    };

    template <typename TResult = void>
    class [[nodiscard("Did you forget to co_await or syncWait the Task<TResult> ?")]] Task {
    public:
        using promise_type = detail::TaskPromise<TResult>;

        using CoroHandleType = std::coroutine_handle<promise_type>;
        using ResultType = TResult;

        constexpr explicit Task(CoroHandleType handle) noexcept :
            mHandle(handle) {}

        constexpr Task(Task &&other) noexcept :
            mHandle(std::exchange(other.mHandle, {})) {}

        ~Task() noexcept {
            if (mHandle) mHandle.destroy();
        }

        Task &operator=(Task &&other) noexcept {
            if (std::addressof(other) != this) {
                if (mHandle) mHandle.destroy();
                mHandle = std::exchange(other.mHandle, {});
            }
            return *this;
        }

        Task(const Task &) = delete;
        Task &operator=(const Task &) = delete;

        constexpr auto operator co_await() const & noexcept {
            struct Awaiter {
                CoroHandleType mTaskCoroHandle;

                constexpr Awaiter(CoroHandleType handle) noexcept : mTaskCoroHandle(handle) {}
                bool await_ready() const noexcept { return !mTaskCoroHandle || mTaskCoroHandle.done(); }

                std::coroutine_handle<> await_suspend(std::coroutine_handle<> callerCoroHandle) noexcept {
                    mTaskCoroHandle.promise().mContinuation = callerCoroHandle;
                    return mTaskCoroHandle;
                }
                decltype(auto) await_resume() { return mTaskCoroHandle.promise().result(); }
            };

            return Awaiter{mHandle};
        }

        constexpr auto operator co_await() const && noexcept {
            struct Awaiter {
                CoroHandleType mTaskCoroHandle;

                constexpr Awaiter(CoroHandleType handle) noexcept : mTaskCoroHandle(handle) {}
                bool await_ready() const noexcept { return !mTaskCoroHandle || mTaskCoroHandle.done(); }

                std::coroutine_handle<> await_suspend(std::coroutine_handle<> callerCoroHandle) noexcept {
                    mTaskCoroHandle.promise().mContinuation = callerCoroHandle;
                    return mTaskCoroHandle;
                }

                decltype(auto) await_resume() { return std::move(mTaskCoroHandle.promise()).result(); }
            };

            return Awaiter{mHandle};
        }

    private:
        CoroHandleType mHandle;
    };

    template <typename TResult>
    Task<TResult> detail::TaskPromise<TResult>::get_return_object() noexcept {
        return Task<TResult>{CoroHandleType::from_promise(*this)};
    }

    inline Task<void> detail::TaskPromise<void>::get_return_object() noexcept {
        return Task<void>{CoroHandleType::from_promise(*this)};
    }

    template <typename TResult>
    Task<TResult &> detail::TaskPromise<TResult &>::get_return_object() noexcept {
        return Task<TResult &>{CoroHandleType::from_promise(*this)};
    }

} // namespace sapphire::coro