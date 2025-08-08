#pragma once

#include <algorithm>

namespace util {

    template <std::size_t N>
    struct StringLiteral {
        static constexpr std::size_t size = N;
        char                         value[N]{};
        consteval StringLiteral(const char (&str)[N]) { std::copy_n(str, N, value); };
    };

} // namespace util