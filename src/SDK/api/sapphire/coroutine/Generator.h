#pragma once

#include <coroutine>
#include <exception>
#include <ranges>

namespace sapphire::coro {

    /// 抄的标准库

    namespace detail {

        template <typename T>
        concept cv_unqualified_object = std::is_object_v<T> && std::same_as<T, std::remove_cv_t<T>>;

        template <typename Yielded, typename Generator>
        class GeneratorPromise {
            using yield_cref_type = const std::remove_reference_t<Yielded> &;
            using yield_value_type = std::remove_cvref_t<Yielded>;
            using pointer_type = std::add_pointer_t<Yielded>;

            struct CopyAwaiter : std::suspend_always {
                yield_value_type value;
                pointer_type    &ptr;
                constexpr void   await_suspend(std::coroutine_handle<>) noexcept { ptr = std::addressof(value); }
            };

        public:
            constexpr std::suspend_always initial_suspend() const noexcept { return {}; }
            constexpr std::suspend_always final_suspend() const noexcept { return {}; }
            Generator                     get_return_object() noexcept;

            std::suspend_always yield_value(Yielded value) noexcept {
                mValue = std::addressof(value);
                return {};
            }

            CopyAwaiter yield_value(
                yield_cref_type value
            ) noexcept(std::is_nothrow_move_constructible_v<yield_value_type>)
                requires(
                    std::is_rvalue_reference_v<Yielded>
                    && std::constructible_from<yield_value_type, yield_cref_type>
                )
            {
                return CopyAwaiter{{}, value, mValue};
            }

            pointer_type &valuePtr() noexcept { return mValue; }

            void return_void() noexcept {}

            void unhandled_exception() noexcept { mException = std::current_exception(); }

            void rethrow_if_exception() {
                if (mException) std::rethrow_exception(mException);
            }

        private:
            pointer_type       mValue = nullptr;
            std::exception_ptr mException = nullptr;
        };

        template <typename Ref, typename Val, typename Promise>
        class GeneratorIterator {
        public:
            using value_type = Val;
            using reference_type = Ref;
            using difference_type = ptrdiff_t;

            GeneratorIterator(std::coroutine_handle<Promise> handle) noexcept :
                mHandle(handle) {}

            GeneratorIterator &operator++() {
                mHandle.resume();
                if (mHandle.done()) {
                    mHandle.promise().rethrow_if_exception();
                }
                return *this;
            }
            void operator++(int) { (void)operator++(); }

            reference_type operator*() const noexcept(std::is_nothrow_move_constructible_v<reference_type>) {
                return static_cast<reference_type>(*mHandle.promise().valuePtr());
            }

            bool operator==(std::default_sentinel_t) const noexcept { return !mHandle || mHandle.done(); }
            bool operator!=(std::default_sentinel_t) const noexcept { return mHandle && !mHandle.done(); }

        private:
            std::coroutine_handle<Promise> mHandle;
        };

    } // namespace detail

    template <typename Ref, typename Val = void>
    class [[nodiscard]] Generator {
        using value_type = std::conditional_t<std::is_void_v<Val>, std::remove_cvref_t<Ref>, Val>;
        static_assert(detail::cv_unqualified_object<value_type>);

        using reference_type = std::conditional_t<std::is_void_v<Val>, Ref &&, Ref>;
        static_assert(
            std::is_reference_v<reference_type>
            || (detail::cv_unqualified_object<reference_type> && std::copy_constructible<reference_type>)
        );

        using RRef = std::conditional_t<
            std::is_reference_v<reference_type>,
            std::remove_reference_t<reference_type> &&,
            reference_type>;

        static_assert(std::common_reference_with<reference_type &&, value_type &&>);
        static_assert(std::common_reference_with<reference_type &&, RRef &&>);
        static_assert(std::common_reference_with<RRef &&, const value_type &>);

    public:
        using yielded_type =
            std::conditional_t<std::is_reference_v<reference_type>, reference_type, const reference_type &>;
        using promise_type = detail::GeneratorPromise<yielded_type, Generator>;
        using iterator = detail::GeneratorIterator<reference_type, value_type, promise_type>;

        iterator begin() {
            if (!mHandle) {
                return iterator{nullptr};
            }
            mHandle.resume();
            if (mHandle.done()) {
                mHandle.promise().rethrow_if_exception();
                return iterator{nullptr};
            }
            return iterator{mHandle};
        }

        std::default_sentinel_t end() const noexcept { return {}; }

        Generator() noexcept = default;

        Generator(Generator &&other) noexcept :
            mHandle(other.mHandle) {
            other.mHandle = nullptr;
        }

        ~Generator() noexcept {
            if (mHandle) mHandle.destroy();
        }

    private:
        friend promise_type;

        Generator(std::coroutine_handle<promise_type> handle) noexcept :
            mHandle(handle) {}

        std::coroutine_handle<promise_type> mHandle;
    };

    template <typename Yielded, typename Generator>
    Generator detail::GeneratorPromise<Yielded, Generator>::get_return_object() noexcept {
        return {std::coroutine_handle<GeneratorPromise>::from_promise(*this)};
    }

} // namespace sapphire::coro