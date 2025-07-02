#pragma once

#include <cstdint>

namespace dragon::rendering {

    struct ColorWriteMask // size: 1
    {
        uint8_t mWriteMask; // off+0

        ColorWriteMask(uint8_t mask = 0) :
            mWriteMask(mask) {}
    };

} // namespace dragon::rendering