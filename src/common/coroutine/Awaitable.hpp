#pragma once

#include <coroutine>
#include <type_traits>
#include <utility>

namespace sapphire::coro {

    namespace detail {

        template <typename T>
        constexpr decltype(auto) getAwaiter(T &&value) {
            if constexpr (requires { std::forward<T>(value).operator co_await(); }) {
                return std::forward<T>(value).operator co_await();
            } else if constexpr (requires { operator co_await(std::forward<T>(value)); }) {
                return operator co_await(std::forward<T>(value));
            } else {
                return std::forward<T>(value);
            }
        }

    } // namespace detail

    template <typename TAwaitable>
    struct GetAwaiter {
        using Type = std::remove_cvref_t<
            decltype(detail::getAwaiter(std::forward<TAwaitable>(std::declval<TAwaitable>())))>;
    };

    template <typename TAwaitable>
    using GetAwaiterType = typename GetAwaiter<TAwaitable>::Type;

    template <typename TCoroutineHandle>
    concept IsCoroutineHandle = requires {
        requires std::default_initializable<TCoroutineHandle>;
        requires std::convertible_to<TCoroutineHandle, std::coroutine_handle<>>;
    };

    template <typename TAwaiter, typename TPromise = void>
    concept IsAwaiter = requires(TAwaiter awaiter, std::coroutine_handle<TPromise> handle) {
        { awaiter.await_ready() } -> std::convertible_to<bool>;
        { awaiter.await_resume() };
        requires(
            requires { { awaiter.await_suspend(handle) } -> std::same_as<void>; }
            || requires { { awaiter.await_suspend(handle) } -> std::same_as<bool>; }
            || requires { { awaiter.await_suspend(handle) } -> IsCoroutineHandle; }
        );
    };

    template <typename TAwaitable, typename TPromise = void>
    concept IsAwaitable = IsAwaiter<GetAwaiterType<TAwaitable>, TPromise>;

    template <typename TAwaitable, typename TPromise = void>
        requires IsAwaitable<TAwaitable, TPromise>
    struct AwaitableTraits {
        using AwaiterType = GetAwaiterType<TAwaitable>;
        using ResultType = decltype(std::declval<AwaiterType>().await_resume());
    };

} // namespace sapphire::coro
