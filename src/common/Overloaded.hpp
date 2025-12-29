#pragma once

namespace util {

    template <typename... Ts>
    struct Overloaded : Ts... {
        using Ts::operator()...;
    };

} // namespace sapphire::util