#pragma once

#include <coroutine>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <semaphore>
#include <type_traits>
#include <variant>
#include "Awaitable.hpp"
#include "TypeTraits.hpp"

namespace sapphire::coro {

    namespace detail {

        template <typename T>
        class SyncWaitTask;

        class SyncWaitEvent {
        public:
            constexpr SyncWaitEvent() = default;
            SyncWaitEvent &operator=(SyncWaitEvent &&) = delete;
            void           set() noexcept { mSignal.release(); }
            void           wait() noexcept { mSignal.acquire(); }

        private:
            std::binary_semaphore mSignal{0};
        };

        template <typename Derived>
        class SyncWaitPromiseBase {
        public:
            using CoroHandleType = std::coroutine_handle<Derived>;

            constexpr std::suspend_always initial_suspend() const noexcept { return {}; }

            constexpr auto final_suspend() noexcept {
                struct Awaiter {
                    constexpr bool await_ready() const noexcept { return false; }
                    constexpr void await_resume() const noexcept {}
                    void           await_suspend(std::coroutine_handle<Derived> handle) const noexcept {
                        handle.promise().mEvent->set();
                    }
                };
                return Awaiter{};
            }

            SyncWaitEvent *mEvent = nullptr;
        };

        template <typename TResult>
        class SyncWaitPromise : public SyncWaitPromiseBase<SyncWaitPromise<TResult>> {
        public:
            SyncWaitTask<TResult> get_return_object() noexcept;

            void unhandled_exception() noexcept {
                mResult.template emplace<2>(std::current_exception());
            }

            void return_value(TResult &&value) noexcept {
                mResult.template emplace<1>(std::addressof(value));
            }

            TResult &&result() {
                if (mResult.index() == 2) std::rethrow_exception(std::get<2>(mResult));
                return static_cast<TResult &&>(*std::get<1>(mResult));
            }

            std::variant<std::monostate, std::remove_reference_t<TResult> *, std::exception_ptr> mResult;
        };

        template <>
        class SyncWaitPromise<void> : public SyncWaitPromiseBase<SyncWaitPromise<void>> {
        public:
            SyncWaitTask<void> get_return_object() noexcept;

            void unhandled_exception() noexcept {
                mResult = std::current_exception();
            }

            constexpr void return_void() noexcept {}

            void result() {
                if (mResult) std::rethrow_exception(mResult);
            }

            std::exception_ptr mResult;
        };

        template <typename TResult>
        class [[nodiscard]] SyncWaitTask {
        public:
            using promise_type = SyncWaitPromise<TResult>;
            using CoroHandleType = std::coroutine_handle<promise_type>;

            explicit SyncWaitTask(CoroHandleType handle) noexcept :
                mHandle(handle) {}

            SyncWaitTask(SyncWaitTask &&other) noexcept :
                mHandle(std::exchange(other.mHandle, {})) {}

            ~SyncWaitTask() noexcept {
                if (mHandle) mHandle.destroy();
            }

            void start(SyncWaitEvent &event) {
                mHandle.promise().mEvent = &event;
                mHandle.resume();
            }

            decltype(auto) result() {
                return mHandle.promise().result();
            }

        public:
            CoroHandleType mHandle;
        };

        template <typename TResult>
        SyncWaitTask<TResult> SyncWaitPromise<TResult>::get_return_object() noexcept {
            return SyncWaitTask<TResult>{SyncWaitPromise<TResult>::CoroHandleType::from_promise(*this)};
        }

        inline SyncWaitTask<void> SyncWaitPromise<void>::get_return_object() noexcept {
            return SyncWaitTask<void>{CoroHandleType::from_promise(*this)};
        }

    } // namespace detail

    template <IsAwaitable TAwaitable, typename TResult = AwaitableTraits<TAwaitable>::ResultType>
    auto syncWait(TAwaitable &&awaitable) -> RemoveRValueReference<TResult> {
        static_assert(
            std::is_reference_v<TResult> || std::is_void_v<TResult>,
            "TAwaitable await_resume() evaluates to prvalue, that is not supported"
        );

        auto task = [](TAwaitable &&awaitable) -> detail::SyncWaitTask<TResult> {
            co_return co_await std::forward<TAwaitable>(awaitable);
        }(std::forward<TAwaitable>(awaitable));

        detail::SyncWaitEvent event;
        task.start(event);
        event.wait();

        return task.result();
    }

} // namespace sapphire::coro