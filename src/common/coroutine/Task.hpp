#pragma once

#include "macros/Macros.h"

#include <cassert>
#include <coroutine>
#include <exception>
#include <optional>
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
                std::coroutine_handle<> await_suspend(std::coroutine_handle<TPromise> h) const noexcept {
                    return h.promise().mContinuation;
                }
                constexpr void await_resume() const noexcept {}
            };

            constexpr TaskFinalAwaiter final_suspend() const noexcept { return {}; }

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
            constexpr void return_value(TResult &&value) noexcept { mResult.template emplace<1>(std::move(value)); }

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

    template <typename Result>
    struct [[nodiscard]] CoTryTask {
        struct promise_type;
        using Handle = std::coroutine_handle<promise_type>;

        struct promise_type : public detail::TaskPromiseBase {
            std::optional<Result> mResult;

            SPHR_FORCE_INLINE CoTryTask get_return_object() noexcept { return CoTryTask{Handle::from_promise(*this)}; }
            SPHR_FORCE_INLINE std::suspend_always initial_suspend() noexcept { return {}; }
            SPHR_FORCE_INLINE TaskFinalAwaiter    final_suspend() noexcept { return {}; }

            template <typename U>
                requires std::is_constructible_v<Result, U>
            SPHR_FORCE_INLINE constexpr void return_value(U &&value) noexcept {
                mResult.emplace(std::forward<U>(value));
            }
            SPHR_FORCE_INLINE constexpr void return_value(Result &&value) noexcept { mResult.emplace(std::move(value)); }
        };

        Handle mHandle;

        SPHR_FORCE_INLINE explicit CoTryTask(Handle h) noexcept : mHandle(h) {}

        CoTryTask(const CoTryTask &) = delete;
        CoTryTask &operator=(const CoTryTask &) = delete;

        SPHR_FORCE_INLINE CoTryTask(CoTryTask &&rhs) noexcept : mHandle(rhs.mHandle) { rhs.mHandle = nullptr; }

        SPHR_FORCE_INLINE ~CoTryTask() {
            if (mHandle) { mHandle.destroy(); }
        }

        struct Awaiter {
            Handle mHandle;

            SPHR_FORCE_INLINE constexpr bool await_ready() const noexcept { return false; }

            SPHR_FORCE_INLINE void await_suspend(std::coroutine_handle<> h) const noexcept {
                mHandle.promise().mContinuation = h;
                mHandle.resume();
            }

            SPHR_FORCE_INLINE Result &await_resume() {
                auto &promise = mHandle.promise();
                return promise.mResult.value();
            }
        };

        SPHR_FORCE_INLINE Awaiter operator co_await() & noexcept { return Awaiter{mHandle}; }
    };

} // namespace sapphire::coro