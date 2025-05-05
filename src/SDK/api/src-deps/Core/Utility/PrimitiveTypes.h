#pragma once

#include <cstdint>

typedef uint8_t byte;

namespace mce {

    enum class PrimitiveMode : byte {
        Quad = 1,
        TriangleFan,
        TriangleStrip,
        Lines
    };

};
