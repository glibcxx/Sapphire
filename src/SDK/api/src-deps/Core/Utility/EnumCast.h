#pragma once

#include <type_traits>

template <typename T>
    requires(!std::is_enum_v<T>)
constexpr decltype(auto) EnumCastHelper(T integral) {
    return integral;
}

template <typename T>
    requires std::is_enum_v<T>
constexpr decltype(auto) EnumCastHelper(T enumValue) {
    return static_cast<std::underlying_type_t<T>>(enumValue);
}

template <typename T>
constexpr decltype(auto) enum_cast(T t) {
    return EnumCastHelper<T>(t);
}