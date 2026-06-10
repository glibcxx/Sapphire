#pragma once

#include "pch.h" // IWYU pragma: keep

namespace dragon::rendering {

    enum class CullMode : int {
        None = 0,
        Clockwise = 1,
        CounterClockwise = 2,
    };

} // namespace dragon::rendering