#pragma once

#include "pch.h" // IWYU pragma: keep

namespace mce {

    // size: 24
    struct ViewportInfo {
        glm::tvec2<float> size;     // off+0
        glm::tvec2<float> offset;   // off+8
        float             minDepth; // off+16
        float             maxDepth; // off+20
    };

} // namespace mce