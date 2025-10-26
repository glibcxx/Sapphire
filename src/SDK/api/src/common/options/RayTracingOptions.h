#pragma once

#include "IAdvancedGraphicsOptions.h"
#include "SDK/api/src/common/resources/ResourcePackListener.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"

// size: 96 (1.21.50)
class RayTracingOptions : public ResourcePackListener, public IAdvancedGraphicsOptions {
public:
    Bedrock::NotNullNonOwnerPtr<IAdvancedGraphicsHardwareOptions> mHardwareOptions;              // off+32
    bool                                                          mRayTracingResourcesAvailable; // off+56
    bool                                                          mPBRResourcesAvailable;        // off+57
    dragon::rendering::LightingModels                             mUnk66;                        // off+60
    char                                                          mUnk64;                        // off+64
    char                                                          mUnk65;                        // off+65
    dragon::rendering::LightingModels                             mLightingModels;               // off+68
    Bedrock::NotNullNonOwnerPtr<ResourcePackManager>              mResourcePackManager;          // off+72
};