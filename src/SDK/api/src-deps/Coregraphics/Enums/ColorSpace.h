#pragma once

#include <cstdint>

namespace cg {

    enum class ColorSpace : uint8_t {
        Unknown = 0,
        sRGB = 1,
        Linear = 2,
    };

}