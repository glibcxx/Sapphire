#pragma once

#include "pch.h" // IWYU pragma: keep

namespace mce::framebuilder {

    class FogDescription {
        glm::tvec4<float> mColor;          // off+0
        glm::tvec2<float> mControl;        // off+16
        float             mRenderDistance; // off+24
    };

} // namespace mce::framebuilder