#pragma once

#include <algorithm>
#include <string_view>

namespace util {

    template <std::size_t N>
    struct StringLiteral {
        static constexpr std::size_t size = N;
        char                         value[N]{};
        consteval StringLiteral(const char (&str)[N]) { std::copy_n(str, N, value); };
        consteval StringLiteral(std::string_view str) { std::copy_n(str.data(), N, value); };
        constexpr std::string_view view() const { return {value, N}; }
    };

} // namespace util