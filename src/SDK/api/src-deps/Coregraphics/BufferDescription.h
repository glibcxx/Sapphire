#pragma once

#include <cstdint>

namespace cg {

    // size: 8
    struct BufferDescription {
        uint32_t mStride; // off+0
        uint32_t mCount;  // off+4
    };

} // namespace cg