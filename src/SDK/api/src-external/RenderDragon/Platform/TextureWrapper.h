#pragma once

#include "pch.h" // IWYU pragma: keep

namespace dragon::platform {

    // size: 4
    struct GLTextureWrapper {
        uint32_t mGLName; // off+0
    };

    // size: 8
    struct WindowsTextureWrapper {
        void *mPtr; // off+0
    };

} // namespace dragon::platform
