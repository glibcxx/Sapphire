#pragma once

#include "SDK/api/src-external/RenderDragon/Rendering/LightingModels.h"

namespace mce::framebuilder {

    // size: 7
    struct FrameLightingModelCapabilities {
        dragon::rendering::LightingModels mLightingModel; // off+0
        bool                              mUnk4;          // off+4
        bool                              mUnk5;          // off+5
        bool                              mUsePBRFormats; // off+6
    };

} // namespace mce::framebuilder