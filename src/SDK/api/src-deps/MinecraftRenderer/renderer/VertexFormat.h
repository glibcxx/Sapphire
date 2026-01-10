#pragma once

#include "SDK/api/src-deps/Core/Utility/WideByteMask.h"
#include <algorithm>

namespace mce {

    // size: 32
    class VertexFormat {
    public:
        // size: 4
        struct FieldOffset {
            uint16_t mOffset; // off+0
            uint16_t mSize;   // off+2
        };

        WideByteMask fieldMask{};            // off+0
        uint16_t     _fieldOffset[14];       // off+2
        uint16_t     vertexSize = 0;         // off+30
        bool         allowHalfFloats = true; // off+32

        VertexFormat() noexcept {
            std::ranges::fill(_fieldOffset, 0xFFFF);
        }
    };

} // namespace mce