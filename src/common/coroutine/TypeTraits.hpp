#pragma once

#include <type_traits>

namespace sapphire::coro {

    template <typename T, bool Const = false>
    constexpr bool isConvertibleToCharSpan =
        std::is_constructible_v<std::span<char>, T> || std::is_constructible_v<std::span<unsigned char>, T>
        || std::is_constructible_v<std::span<signed char>, T>;

    template <typename T>
    constexpr bool isConvertibleToCharSpan<T, true> =
        std::is_constructible_v<std::span<const char>, T> || std::is_constructible_v<std::span<const unsigned char>, T>
        || std::is_constructible_v<std::span<const signed char>, T>;

    template <typename T>
    using RemoveRValueReference = std::conditional_t<std::is_rvalue_reference_v<T>, std::remove_reference_t<T>, T>;

} // namespace sapphire::coro