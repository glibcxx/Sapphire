#pragma once

#include "TypeTraits.hpp"

namespace sapphire::refl {

    template <typename T>
    concept MembCountable = isMembCountable<T>;

    template <typename T>
    concept MembReflectable = isMembReflectable<T>;

} // namespace sapphire::refl