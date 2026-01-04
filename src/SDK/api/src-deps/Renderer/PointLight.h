#pragma once

namespace mce {

    // size: 32
    struct PointLight {
        float             mIntensity;     // off+0
        glm::tvec3<float> mColor;         // off+4
        glm::tvec3<float> mWorldPosition; // off+16
        bool              mIsLarge;       // off+28
    };

} // namespace mce