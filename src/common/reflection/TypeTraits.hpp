#pragma once

#include <type_traits>

namespace sapphire::refl {

    template <typename T>
    constexpr bool isMembCountable = std::is_aggregate_v<std::remove_cvref_t<T>>;

    template <typename T>
    constexpr bool isMembReflectable = isMembCountable<T>
                                  && std::is_default_constructible_v<std::remove_cvref_t<T>>
                                  && std::is_standard_layout_v<std::remove_cvref_t<T>>;

} // namespace sapphire::refl