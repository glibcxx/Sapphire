#pragma once

#include "pch.h" // IWYU pragma: keep

namespace bgfx {

    // size: 1
    struct RenderFrame {
        enum class Enum : int {
            NoContext = 0,
            Render = 1,
            Timeout = 2,
            Exiting = 3,
            Count = 4,
        };
    };

    // size: 48 (1.21.50)
    struct PlatformData {
        void    *ndt;          // off+0
        void    *nwh;          // off+8
        void    *context;      // off+16
        void    *backBuffer;   // off+24
        void    *backBufferDS; // off+32
        uint64_t flags;        // off+40
    };

} // namespace bgfx