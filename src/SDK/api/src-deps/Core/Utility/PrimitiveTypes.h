#pragma once

#include "pch.h" // IWYU pragma: keep

typedef uint8_t byte;

namespace mce {

    enum class PrimitiveMode : byte {
        None = 0,
        QuadList = 1,
        TriangleList = 2,
        TriangleStrip = 3,
        LineList = 4,
        LineStrip = 5,
    };

};
