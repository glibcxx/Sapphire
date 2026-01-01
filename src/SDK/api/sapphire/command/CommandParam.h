#pragma once

namespace sapphire::command {

    struct text {};

#define SPHR_CMD_TEXT(name) \
    [[msvc::no_unique_address]] ::sapphire::command::text name

    template <typename T>
    struct opt {
        using Type = T;
        T mValue;

        constexpr opt &operator=(const T &value) noexcept(std::is_nothrow_copy_assignable_v<T>) {
            mValue = value;
            return *this;
        }
        constexpr opt &operator=(T &&value) noexcept(std::is_nothrow_move_assignable_v<T>) {
            mValue = std::move(value);
            return *this;
        }
        constexpr operator T &() noexcept { return mValue; }
        constexpr operator const T &() const noexcept { return mValue; }

        constexpr T       &value() noexcept { return mValue; }
        constexpr const T &value() const noexcept { return mValue; }
        constexpr T       &operator*() noexcept { return mValue; }
        constexpr const T &operator*() const noexcept { return mValue; }
        constexpr T       *operator->() noexcept { return &mValue; }
        constexpr const T *operator->() const noexcept { return &mValue; }
    };

    template <typename T>
    constexpr bool isOpt = false;

    template <typename T>
    constexpr bool isOpt<opt<T>> = true;

    template <typename T>
    struct RemoveOpt {
        using Type = T;
    };

    template <typename T>
    struct RemoveOpt<opt<T>> {
        using Type = T;
    };

    template <typename T>
    using RemoveOptT = RemoveOpt<T>::Type;

} // namespace sapphire::command