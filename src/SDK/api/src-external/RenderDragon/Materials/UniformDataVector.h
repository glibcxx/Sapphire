#pragma once

#include <cstdint>

namespace dragon::materials {

    // size: 16
    template <typename T>
    class UniformDataVector {
    public:
        uint8_t *mDataPtr;  // off+0
        uint16_t mCapacity; // off+8
        uint16_t mSize;     // off+10
    };

} // namespace dragon::materials