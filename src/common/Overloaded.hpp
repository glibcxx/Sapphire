#pragma once

namespace sapphire {

    template <typename... Ts>
    struct Overloaded : Ts... {
        using Ts::operator()...;
    };

} // namespace sapphire::util