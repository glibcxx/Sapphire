#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

namespace sapphire {

    template <typename E>
    class Unexpected;

    template <typename T, typename E>
    class Expected;

    namespace detail {

        struct UnexpectedTag {};
        struct InPlaceTag {};

        template <typename T, typename E>
        class ExpectedStorage;

        template <typename T>
        constexpr bool isUnexpected = false;
        template <typename E>
        constexpr bool isUnexpected<Unexpected<E>> = true;
        template <typename T>
        constexpr bool isExpected = false;
        template <typename T, typename E>
        constexpr bool isExpected<Expected<T, E>> = true;

        // vvv Copied from GCC vvv
        template <typename U>
        struct Guard {
            static_assert(std::is_nothrow_move_constructible_v<U>);
            constexpr explicit Guard(U &u) : mGuarded(std::addressof(u)), mTemp(std::move(u)) {
                std::destroy_at(mGuarded);
            }
            constexpr ~Guard() {
                if (mGuarded) [[unlikely]]
                    std::construct_at(mGuarded, std::move(mTemp));
            }

            Guard(const Guard &) = delete;
            Guard &operator=(const Guard &) = delete;

            constexpr U &&release() noexcept {
                mGuarded = nullptr;
                return std::move(mTemp);
            }

        private:
            U *mGuarded;
            U  mTemp;
        };

        template <typename T, typename U, typename Arg>
        constexpr void reinit(T *newVal, U *oldVal, Arg &&arg) noexcept(
            std::is_nothrow_constructible_v<T, Arg> || std::is_nothrow_move_constructible_v<T>
            || std::is_nothrow_move_constructible_v<U>
        ) {
            if constexpr (std::is_nothrow_constructible_v<T, Arg>) {
                std::destroy_at(oldVal);
                std::construct_at(newVal, std::forward<Arg>(arg));
            } else if constexpr (std::is_nothrow_move_constructible_v<T>) {
                T temp(std::forward<Arg>(arg));
                std::destroy_at(oldVal);
                std::construct_at(newVal, std::move(temp));
            } else {
                Guard<U> g(*oldVal);
                std::construct_at(newVal, std::forward<Arg>(arg));
                g.release();
            }
        }
        // ^^^ Copied from GCC ^^^

    } // namespace detail

    inline constexpr detail::UnexpectedTag unexpected;

    inline constexpr detail::InPlaceTag inPlace;

    template <typename E>
    class [[nodiscard]] Unexpected {
        E mError;

    public:
        template <typename T2, typename E2>
        friend class detail::ExpectedStorage;

        template <typename E2 = E>
            requires std::is_constructible_v<E, E2>
        constexpr Unexpected(E2 &&e) noexcept(std::is_nothrow_constructible_v<E, E2>) : mError(std::forward<E2>(e)) {}

        template <typename... Args>
            requires std::is_constructible_v<E, Args...>
        constexpr Unexpected(detail::InPlaceTag, Args &&...args) noexcept(std::is_nothrow_constructible_v<E, Args...>) :
            mError(std::forward<Args>(args)...) {}

        template <typename... Args>
            requires std::is_constructible_v<E, Args...>
        constexpr Unexpected(std::in_place_t, Args &&...args) noexcept(std::is_nothrow_constructible_v<E, Args...>) :
            mError(std::forward<Args>(args)...) {}

        [[nodiscard]] constexpr E &error() noexcept { return mError; }

        [[nodiscard]] constexpr const E &error() const noexcept { return mError; }
    };

    template <typename E>
    Unexpected(E) -> Unexpected<E>;

    /////////////////////////////////

    template <typename T, typename E>
    class [[nodiscard]] Expected : public detail::ExpectedStorage<T, E> {
        using StorageType = detail::ExpectedStorage<T, E>;

    public:
        static_assert(!std::is_reference_v<T> && !std::is_function_v<T> && !std::is_array_v<T>);
        static_assert(!std::is_const_v<T> && !std::is_volatile_v<T>);

        using StorageType::StorageType;
        using StorageType::operator=;

        constexpr bool hasValue() const noexcept { return this->mHasValue; }
        constexpr bool hasError() const noexcept { return !this->mHasValue; }

        [[nodiscard]] constexpr decltype(auto) value() noexcept {
            assert(hasValue());
            if constexpr (!std::is_void_v<T>) return (this->mValue);
        }

        [[nodiscard]] constexpr decltype(auto) value() const noexcept {
            assert(hasValue());
            if constexpr (!std::is_void_v<T>) return (this->mValue);
        }

        [[nodiscard]] constexpr E &error() noexcept {
            assert(hasError());
            return this->mError;
        }

        [[nodiscard]] constexpr const E &error() const noexcept {
            assert(hasError());
            return this->mError;
        }

        explicit constexpr operator bool() const noexcept { return hasValue(); }

        [[nodiscard]] constexpr decltype(auto) operator*() noexcept { return value(); }
        [[nodiscard]] constexpr decltype(auto) operator*() const noexcept { return value(); }

        [[nodiscard]] constexpr decltype(auto) operator->() noexcept {
            if constexpr (!std::is_void_v<T>) return &value();
        }
        [[nodiscard]] constexpr decltype(auto) operator->() const noexcept {
            if constexpr (!std::is_void_v<T>) return &value();
        }
    };

    template <typename T, typename E>
    class detail::ExpectedStorage {
    protected:
        union {
            std::remove_cv_t<T> mValue;
            E                   mError;
        };
        bool mHasValue;

    public:
        template <typename T2, typename E2>
        friend class ExpectedStorage;

        // 默认构造

        constexpr ExpectedStorage() noexcept(std::is_nothrow_default_constructible_v<T>)
            requires std::is_default_constructible_v<T>
            : mValue(), mHasValue(true) {}

        // 禁止指针到 bool 的隐式转换
        template <typename U>
        constexpr ExpectedStorage(U *value)
            requires(std::same_as<T, bool>)
        = delete;

        // 有参构造

        // 通用赋值
        template <typename U>
            requires(!std::is_same_v<std::remove_cvref_t<U>, ExpectedStorage>
                     && !std::is_same_v<std::remove_cvref_t<U>, std::in_place_t>
                     && !std::is_same_v<std::remove_cvref_t<U>, InPlaceTag>
                     && !std::is_same_v<std::remove_cvref_t<U>, UnexpectedTag> && std::is_constructible_v<T, U>
                     && !detail::isUnexpected<std::remove_cvref_t<U>>
                     && !(std::is_same_v<std::remove_cv_t<T>, bool> && detail::isExpected<std::remove_cvref_t<U>>))
        constexpr ExpectedStorage(U &&value) noexcept(std::is_nothrow_constructible_v<T, U>) :
            mValue(std::forward<U>(value)), mHasValue(true) {}

        // 原地构造
        template <typename... Args>
        constexpr ExpectedStorage(InPlaceTag, Args &&...args) noexcept(std::is_nothrow_constructible_v<T, Args...>) :
            mValue(std::forward<Args>(args)...), mHasValue(true) {}
        template <typename... Args>
        constexpr ExpectedStorage(
            std::in_place_t, Args &&...args
        ) noexcept(std::is_nothrow_constructible_v<T, Args...>) :
            mValue(std::forward<Args>(args)...), mHasValue(true) {}

        // Unexpected 构造

        // 复制 Unexpected
        template <typename E2>
        constexpr ExpectedStorage(const Unexpected<E2> &ue) noexcept(std::is_nothrow_constructible_v<E, const E2 &>) :
            mError(ue.mError), mHasValue(false) {}

        // 移动 Unexpected
        template <typename E2>
        constexpr ExpectedStorage(Unexpected<E2> &&ue) noexcept(std::is_nothrow_constructible_v<E, E2 &&>) :
            mError(std::move(ue.mError)), mHasValue(false) {}

        // UnexpectedTag
        template <typename... Args>
        constexpr ExpectedStorage(UnexpectedTag, Args &&...args) noexcept(std::is_nothrow_constructible_v<E, Args...>) :
            mError(std::forward<Args>(args)...), mHasValue(false) {}

        // 拷贝构造

        // 平凡拷贝
        constexpr ExpectedStorage(const ExpectedStorage &other)
            requires std::is_trivially_copy_constructible_v<T> && std::is_trivially_copy_constructible_v<E>
        = default;

        // 非平凡拷贝
        constexpr ExpectedStorage(
            const ExpectedStorage &other
        ) noexcept(std::is_nothrow_constructible_v<T, const T &> && std::is_nothrow_constructible_v<E, const E &>)
            requires std::is_copy_constructible_v<T> && std::is_copy_constructible_v<E>
                  && (!std::is_trivially_copy_constructible_v<T> || !std::is_trivially_copy_constructible_v<E>)
            : mHasValue(other.mHasValue) {
            if (mHasValue) {
                std::construct_at(std::addressof(mValue), other.mValue);
            } else {
                std::construct_at(std::addressof(mError), other.mError);
            }
        }

        // 跨类型拷贝
        template <typename T2, typename E2>
            requires std::is_constructible_v<T, const T2 &> && std::is_constructible_v<E, const E2 &>
        constexpr ExpectedStorage(
            const ExpectedStorage<T2, E2> &other
        ) noexcept(std::is_nothrow_constructible_v<T, const T2 &> && std::is_nothrow_constructible_v<E, const E2 &>) :
            mHasValue(other.mHasValue) {
            if (mHasValue) {
                std::construct_at(std::addressof(mValue), other.mValue);
            } else {
                std::construct_at(std::addressof(mError), other.mError);
            }
        }

        // 移动构造

        // 平凡移动
        constexpr ExpectedStorage(ExpectedStorage &&other)
            requires std::is_trivially_move_constructible_v<T> && std::is_trivially_move_constructible_v<E>
        = default;

        // 非平凡移动
        constexpr ExpectedStorage(
            ExpectedStorage &&other
        ) noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>) :
            mHasValue(other.mHasValue) {
            if (mHasValue) {
                std::construct_at(std::addressof(mValue), std::move(other.mValue));
            } else {
                std::construct_at(std::addressof(mError), std::move(other.mError));
            }
        }

        // 跨类型移动
        template <typename T2, typename E2>
            requires std::is_constructible_v<T, T2> && std::is_constructible_v<E, E2>
        constexpr ExpectedStorage(
            ExpectedStorage<T2, E2> &&other
        ) noexcept(std::is_nothrow_constructible_v<T, T2> && std::is_nothrow_constructible_v<E, E2>) :
            mHasValue(other.mHasValue) {
            if (mHasValue) {
                std::construct_at(std::addressof(mValue), std::move(other.mValue));
            } else {
                std::construct_at(std::addressof(mError), std::move(other.mError));
            }
        }

        // 有参赋值

        // 通用赋值
        template <typename U>
            requires(
                !std::is_same_v<std::remove_cvref_t<U>, ExpectedStorage> && std::is_constructible_v<T, U>
                && !detail::isUnexpected<std::remove_cvref_t<U>> && std::is_assignable_v<T &, U>
            )
        constexpr ExpectedStorage &operator=(U &&value) {
            _assignValue(std::forward<U>(value));
            return *this;
        }

        // Unexpected 赋值

        // 复制 Unexpected
        template <typename E2>
            requires std::is_constructible_v<E, const E2 &> && std::is_assignable_v<E &, const E2 &>
        constexpr ExpectedStorage &operator=(const Unexpected<E2> &ue) {
            _assignError(ue.mError);
            return *this;
        }

        // 移动 Unexpected
        template <typename E2>
        constexpr ExpectedStorage &operator=(Unexpected<E2> &&ue) {
            _assignError(std::move(ue.mError));
            return *this;
        }

        // 拷贝赋值

        // 平凡拷贝赋值
        constexpr ExpectedStorage &operator=(const ExpectedStorage &other)
            requires std::is_trivially_copy_constructible_v<T> && std::is_trivially_copy_constructible_v<E>
                      && std::is_trivially_copy_assignable_v<T> && std::is_trivially_copy_assignable_v<E>
        = default;

        // 非平凡拷贝赋值
        constexpr ExpectedStorage &operator=(const ExpectedStorage &other) noexcept(
            std::is_nothrow_move_constructible_v<T> && std::is_nothrow_move_constructible_v<E>
            && std::is_nothrow_move_assignable_v<T> && std::is_nothrow_move_assignable_v<E>
        )
            requires(
                std::is_copy_constructible_v<T> && std::is_copy_constructible_v<E> && std::is_copy_assignable_v<T>
                && std::is_copy_assignable_v<E>
                && !(
                    std::is_trivially_copy_constructible_v<T> && std::is_trivially_copy_constructible_v<E>
                    && std::is_trivially_copy_assignable_v<T> && std::is_trivially_copy_assignable_v<E>
                )
            )
        {
            if (other.mHasValue)
                _assignValue(other.mValue);
            else
                _assignError(other.mError);
            return *this;
        }

        // 移动赋值

        // 平凡移动赋值
        constexpr ExpectedStorage &operator=(ExpectedStorage &&other)
            requires std::is_trivially_move_assignable_v<T> && std::is_trivially_move_assignable_v<E>
        = default;

        // 非平凡移动赋值
        constexpr ExpectedStorage &operator=(ExpectedStorage &&other)
            requires(
                std::is_move_constructible_v<T> && std::is_move_constructible_v<E> && std::is_move_assignable_v<T>
                && std::is_move_assignable_v<E>
                && !(std::is_trivially_move_assignable_v<T> && std::is_trivially_move_assignable_v<E>)
            )
        {
            if (other.mHasValue)
                _assignValue(std::move(other.mValue));
            else
                _assignError(std::move(other.mError));
            return *this;
        }

        // 析构

        // 平凡析构
        constexpr ~ExpectedStorage() noexcept
            requires(std::is_trivially_destructible_v<T> && std::is_trivially_destructible_v<E>)
        = default;

        // 非平凡析构
        constexpr ~ExpectedStorage() noexcept {
            if (mHasValue)
                std::destroy_at(std::addressof(mValue));
            else
                std::destroy_at(std::addressof(mError));
        }

    private:
        template <typename U>
            requires std::is_nothrow_constructible_v<T, U> || std::is_nothrow_move_constructible_v<T>
                  || std::is_nothrow_move_constructible_v<E>
        constexpr void _assignValue(U &&value) {
            if (mHasValue)
                mValue = std::forward<U>(value);
            else
                reinit(std::addressof(mValue), std::addressof(mError), std::forward<U>(value));
            mHasValue = true;
        }

        template <typename U>
        constexpr void _assignError(U &&error)
            requires std::is_nothrow_constructible_v<E, U> || std::is_nothrow_move_constructible_v<E>
                  || std::is_nothrow_move_constructible_v<T>
        {
            if (mHasValue)
                reinit(std::addressof(mError), std::addressof(mValue), std::forward<U>(error));
            else
                mError = std::forward<U>(error);
            mHasValue = false;
        }
    };

    template <typename E>
    class detail::ExpectedStorage<void, E> {
    protected:
        union {
            E mError;
        };
        bool mHasValue;

    public:
        template <typename T2, typename E2>
        friend class ExpectedStorage;

        // 默认构造
        constexpr ExpectedStorage() noexcept : mHasValue(true) {}

        // Unexpected 构造

        template <typename E2>
        constexpr ExpectedStorage(const Unexpected<E2> &ue) noexcept(std::is_nothrow_constructible_v<E, const E2 &>) :
            mError(ue.mError), mHasValue(false) {}

        template <typename E2>
        constexpr ExpectedStorage(Unexpected<E2> &&ue) noexcept(std::is_nothrow_constructible_v<E, E2 &&>) :
            mError(std::move(ue.mError)), mHasValue(false) {}

        template <typename... Args>
        constexpr ExpectedStorage(
            detail::UnexpectedTag, Args &&...args
        ) noexcept(std::is_nothrow_constructible_v<E, Args...>) :
            mError(std::forward<Args>(args)...), mHasValue(false) {}

        // 拷贝构造

        constexpr ExpectedStorage(const ExpectedStorage &other)
            requires std::is_trivially_copy_constructible_v<E>
        = default;

        constexpr ExpectedStorage(const ExpectedStorage &other) noexcept(std::is_nothrow_constructible_v<E, const E &>)
            requires std::is_copy_constructible_v<E> && (!std::is_trivially_copy_constructible_v<E>)
            : mHasValue(other.mHasValue) {
            if (!mHasValue) { std::construct_at(std::addressof(mError), other.mError); }
        }

        template <typename T2, typename E2>
            requires std::is_constructible_v<E, const E2 &>
        constexpr ExpectedStorage(
            const ExpectedStorage<T2, E2> &other
        ) noexcept(std::is_nothrow_constructible_v<E, const E2 &>) :
            mHasValue(other.mHasValue) {
            if (!mHasValue) std::construct_at(std::addressof(mError), other.mError);
        }

        // 移动构造

        constexpr ExpectedStorage(ExpectedStorage &&other)
            requires std::is_trivially_move_constructible_v<E>
        = default;

        template <typename T2, typename E2>
        constexpr ExpectedStorage(ExpectedStorage<T2, E2> &&other) noexcept(std::is_nothrow_constructible_v<E, E2>) :
            mHasValue(other.mHasValue) {
            if (!mHasValue) std::construct_at(std::addressof(mError), std::move(other.mError));
        }

        // 从 Expected 构造

        template <typename T2, typename E2>
        constexpr ExpectedStorage(
            const Expected<T2, E2> &other
        ) noexcept(std::is_nothrow_constructible_v<E, const E2 &>) :
            mHasValue(other.mHasValue) {
            if (!mHasValue) std::construct_at(std::addressof(mError), other.mError);
        }

        template <typename T2, typename E2>
        constexpr ExpectedStorage(Expected<T2, E2> &&other) noexcept(std::is_nothrow_constructible_v<E, E2>) :
            mHasValue(other.mHasValue) {
            if (!mHasValue) std::construct_at(std::addressof(mError), std::move(other.mError));
        }

        // Unexpected 赋值

        template <typename E2>
        constexpr ExpectedStorage &
        operator=(const Unexpected<E2> &ue) noexcept(std::is_nothrow_assignable_v<E, const E2 &>) {
            _assignError(ue.mError);
            return *this;
        }

        template <typename E2>
        constexpr ExpectedStorage &operator=(Unexpected<E2> &&ue) noexcept(std::is_nothrow_assignable_v<E, E2>) {
            _assignError(std::move(ue.mError));
            return *this;
        }

        // 拷贝赋值

        constexpr ExpectedStorage &operator=(const ExpectedStorage &other)
            requires std::is_trivially_copy_constructible_v<E> && std::is_trivially_copy_assignable_v<E>
        = default;

        constexpr ExpectedStorage &operator=(const ExpectedStorage &other)
            requires(
                std::is_copy_constructible_v<E> && std::is_copy_assignable_v<E>
                && !(std::is_trivially_copy_constructible_v<E> && std::is_trivially_copy_assignable_v<E>)
            )
        {
            if (other.mHasValue)
                _assignValue();
            else
                _assignError(other.mError);
            return *this;
        }

        // 移动赋值

        constexpr ExpectedStorage &operator=(ExpectedStorage &&other)
            requires std::is_trivially_move_assignable_v<E>
        = default;

        constexpr ExpectedStorage &operator=(ExpectedStorage &&other)
            requires(
                std::is_move_constructible_v<E> && std::is_move_assignable_v<E>
                && !(std::is_trivially_move_assignable_v<E>)
            )
        {
            if (other.mHasValue)
                _assignValue();
            else
                _assignError(std::move(other.mError));
            return *this;
        }

        // 析构

        constexpr ~ExpectedStorage() noexcept
            requires std::is_trivially_destructible_v<E>
        = default;

        constexpr ~ExpectedStorage() noexcept {
            if (!mHasValue) std::destroy_at(std::addressof(mError));
        }

    private:
        constexpr void _assignValue() {
            if (!mHasValue) std::destroy_at(std::addressof(mError));
            mHasValue = true;
        }

        template <typename U>
        constexpr void _assignError(U &&error) {
            if (mHasValue)
                std::construct_at(std::addressof(mError), std::forward<U>(error));
            else
                mError = std::forward<U>(error);
            mHasValue = false;
        }
    };

} // namespace sapphire