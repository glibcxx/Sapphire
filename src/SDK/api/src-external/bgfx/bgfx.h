#pragma once

#include <cstdint>

namespace bgfx {

    // size: 2
    struct TextureHandle {
        uint16_t idx; // off+0
    };

    // size: 16
    struct DLSSOptions {
        enum class Mode : int {
            DLSS = 0,
            Fallback = 1,
            Off = 2,
            Count = 3,
        };

        enum class Scaling : int {
            Performance = 0,
            Balanced = 1,
            Quality = 2,
            UltraPerformance = 3,
            UltraQuality = 4,
            Off = 5,
            Count = 6,
        };

        Mode    mMode;       // off+0
        Scaling mQuality;    // off+4
        float   mSharpness;  // off+8
        float   mMipmapBias; // off+12
    };

} // namespace bgfx