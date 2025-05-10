#pragma once

#include <utility>

namespace util {

    template <std::size_t N>
    struct StringLiteral {
        char value[N]{};
        consteval StringLiteral(const char (&str)[N]) { std::copy_n(str, N, value); };
    };

} // namespace util