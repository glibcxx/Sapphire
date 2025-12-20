#pragma once

#include <atomic>
#include <coroutine>
#include <exception>
#include <format>
#include <iostream>
#include <ranges>
#include <utility>
#include <variant>
#include <vector>
#include "Awaitable.hpp"
#include "TypeTraits.hpp"

namespace sapphire::coro {

    namespace detail {

        template <IsAwaitable TAwaitable>
        class WhenAllTask;

        class WhenAllCounter {
        public:
            explicit WhenAllCounter(size_t count) :
                mCount(count + 1), mAwaitingCoroutine(nullptr) {}

            constexpr bool isReady() const noexcept {
                return mAwaitingCoroutine.operator bool();
            }

            bool tryAwait(std::coroutine_handle<> awaitingCoroutine) noexcept {
                mAwaitingCoroutine = awaitingCoroutine;
                return mCount.fetch_sub(1, std::memory_order_acq_rel) > 1;
            }

            void notifyCompleted() noexcept {
                if (mCount.fetch_sub(1, std::memory_order_acq_rel) == 1) {
                    mAwaitingCoroutine.resume();
                }
            }

        private:
            std::atomic<size_t>     mCount;
            std::coroutine_handle<> mAwaitingCoroutine;
        };

        struct WhenAllTaskPromiseBase {
            struct OnFinalAwaiter : public std::suspend_always {
                void await_suspend(auto coro) const noexcept {
                    coro.promise().mCounter->notifyCompleted();
                }
            };

            constexpr auto initial_suspend() const noexcept { return std::suspend_always{}; }
            constexpr auto final_suspend() const noexcept {
                return OnFinalAwaiter{};
            }

            WhenAllCounter *mCounter = nullptr;
        };

        template <typename TResult>
        struct WhenAllTaskPromise : public WhenAllTaskPromiseBase {
            using CoroHandleType = std::coroutine_handle<WhenAllTaskPromise>;
            auto get_return_object() noexcept {
                return CoroHandleType::from_promise(*this);
            }

            void unhandled_exception() noexcept { mResult = std::current_exception(); }
            void return_value(TResult &&value) noexcept {
                mResult.template emplace<1>(std::addressof(value));
            }
            auto result() const -> RemoveRValueReference<TResult> {
                if (mResult.index() == 2) std::rethrow_exception(std::get<2>(mResult));
                return static_cast<TResult &&>(*std::get<1>(mResult));
            }

            std::variant<std::monostate, std::remove_reference_t<TResult> *, std::exception_ptr> mResult;
        };

        template <>
        struct WhenAllTaskPromise<void> : public WhenAllTaskPromiseBase {
            using CoroHandleType = std::coroutine_handle<WhenAllTaskPromise<void>>;
            auto get_return_object() noexcept {
                return CoroHandleType::from_promise(*this);
            }
            void           unhandled_exception() noexcept { mResult = std::current_exception(); }
            constexpr void return_void() const noexcept {}

            void result() const {
                if (mResult) std::rethrow_exception(mResult);
            }

            std::exception_ptr mResult;
        };

        template <IsAwaitable TAwaitable>
        class WhenAllTask {
        public:
            using promise_type = WhenAllTaskPromise<typename AwaitableTraits<TAwaitable>::ResultType>;

            constexpr WhenAllTask(std::coroutine_handle<promise_type> h) noexcept :
                mHandle(h) {}

            WhenAllTask(const WhenAllTask &) = delete;
            WhenAllTask &operator=(const WhenAllTask &) = delete;

            WhenAllTask(WhenAllTask &&other) noexcept :
                mHandle(std::exchange(other.mHandle, {})) {}

            WhenAllTask operator=(WhenAllTask &&other) noexcept {
                if (std::addressof(other) != this) {
                    if (mHandle) mHandle.destroy();
                    mHandle = std::exchange(other.mHandle, {});
                }
                return *this;
            }

            ~WhenAllTask() noexcept {
                if (mHandle) mHandle.destroy();
            }

            decltype(auto) result() const { return mHandle.promise().result(); }

        private:
            template <IsAwaitable>
            friend class WhenAllAwaitable;
            template <IsAwaitable...>
            friend class WhenAllAwaitableTuple;

            void start(WhenAllCounter &counter) const noexcept {
                mHandle.promise().mCounter = &counter;
                mHandle.resume();
            }

            std::coroutine_handle<promise_type> mHandle;
        };

        template <IsAwaitable TAwaitable>
        class WhenAllAwaitable {
            std::vector<WhenAllTask<TAwaitable>> mTasks;
            WhenAllCounter                       mCounter;

            struct WhenAllAwaiterBase {
                WhenAllAwaitable &mThis;
                constexpr bool    await_ready() const noexcept { return mThis.mCounter.isReady(); }
                bool              await_suspend(std::coroutine_handle<> h) noexcept {
                    for (auto &&t : mThis.mTasks) {
                        t.start(mThis.mCounter);
                    }
                    return mThis.mCounter.tryAwait(h);
                }
            };

        public:
            WhenAllAwaitable(std::vector<TAwaitable> &&tasks) :
                mCounter(tasks.size()) {
                mTasks.reserve(tasks.size());
                for (auto &&t : tasks) {
                    mTasks.emplace_back(
                        [](TAwaitable task) -> WhenAllTask<TAwaitable> {
                            co_return co_await std::move(task);
                        }(std::move(t))
                    );
                }
            }

            WhenAllAwaitable(const WhenAllAwaitable &) = delete;
            WhenAllAwaitable(WhenAllAwaitable &&other) noexcept(std::is_nothrow_move_constructible_v<TAwaitable>) :
                mTasks(std::move(other.mTasks)), mCounter(mTasks.size()) {}

            constexpr auto operator co_await() & noexcept {
                struct WhenAllAwaiter : WhenAllAwaiterBase {
                    constexpr std::vector<WhenAllTask<TAwaitable>> &await_resume() noexcept {
                        return this->mThis.mTasks;
                    }
                };
                return WhenAllAwaiter{*this};
            }

            constexpr auto operator co_await() && noexcept {
                struct WhenAllAwaiter : WhenAllAwaiterBase {
                    constexpr std::vector<WhenAllTask<TAwaitable>> &&await_resume() noexcept {
                        return std::move(this->mThis.mTasks);
                    }
                };
                return WhenAllAwaiter{*this};
            }
        };

        template <IsAwaitable... TAwaitable>
        class WhenAllAwaitableTuple {
            std::tuple<WhenAllTask<TAwaitable>...> mTasks;
            WhenAllCounter                         mCounter;

            struct WhenAllAwaiterBase {
                WhenAllAwaitableTuple &mThis;
                constexpr bool         await_ready() const noexcept { return mThis.mCounter.isReady(); }
                bool                   await_suspend(std::coroutine_handle<> h) noexcept {
                    [&]<std::size_t... INDICES>(std::integer_sequence<std::size_t, INDICES...>) {
                        auto _ = {(std::get<INDICES>(mThis.mTasks).start(mThis.mCounter), 0)...};
                    }(std::make_integer_sequence<std::size_t, sizeof...(TAwaitable)>{});
                    return mThis.mCounter.tryAwait(h);
                }
            };

        public:
            WhenAllAwaitableTuple(TAwaitable &&...tasks) noexcept(
                std::is_nothrow_move_constructible_v<std::tuple<TAwaitable...>>
            ) :
                mTasks([](TAwaitable task) -> WhenAllTask<TAwaitable> {
                    co_return co_await std::move(task);
                }(std::move(tasks))...),
                mCounter(sizeof...(TAwaitable)) {}

            WhenAllAwaitableTuple(const WhenAllAwaitableTuple &) = delete;
            WhenAllAwaitableTuple(WhenAllAwaitableTuple &&other) noexcept(
                std::is_nothrow_move_constructible_v<std::tuple<WhenAllTask<TAwaitable>...>>
            ) : mTasks(other.mTasks),
                mCounter(sizeof...(TAwaitable)) {
            }

            constexpr auto operator co_await() & noexcept {
                struct WhenAllAwaiter : WhenAllAwaiterBase {
                    constexpr std::tuple<WhenAllTask<TAwaitable>...> &await_resume() noexcept {
                        return this->mThis.mTasks;
                    }
                };
                return WhenAllAwaiter{*this};
            }

            constexpr auto operator co_await() && noexcept {
                struct WhenAllAwaiter : WhenAllAwaiterBase {
                    constexpr std::tuple<WhenAllTask<TAwaitable>...> &&await_resume() noexcept {
                        return std::move(this->mThis.mTasks);
                    }
                };
                return WhenAllAwaiter{*this};
            }
        };

    } // namespace detail

    template <IsAwaitable TAwaitable>
    [[nodiscard]] auto whenAll(std::vector<TAwaitable> &&tasks) {
        return detail::WhenAllAwaitable<TAwaitable>(std::move(tasks));
    }

    template <typename TContainer>
        requires std::ranges::range<TContainer> && IsAwaitable<std::ranges::range_value_t<TContainer>>
    [[nodiscard]] auto whenAll(TContainer &&tasks) {
        using TAwaitable = std::ranges::range_value_t<TContainer>;
        std::vector<TAwaitable> taskVector;
        if constexpr (std::ranges::sized_range<TContainer>) {
            taskVector.reserve(std::ranges::size(tasks));
        }
        for (auto &&task : tasks) {
            taskVector.emplace_back(std::forward<decltype(task)>(task));
        }
        return whenAll(std::move(taskVector));
    }

    template <IsAwaitable... TAwaitables>
    [[nodiscard]] auto whenAll(TAwaitables &&...tasks) {
        return detail::WhenAllAwaitableTuple<TAwaitables...>(std::forward<TAwaitables>(tasks)...);
    }

} // namespace sapphire::coro