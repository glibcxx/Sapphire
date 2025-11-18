#pragma once

#include "BaseLightData.h"
#include "macros/Macros.h"

class MinecraftGameplayGraphicsResources;
namespace mce {
    class Image;
    class ClientTexture;
    enum class DynamicTexture : uint8_t;
} // namespace mce

// size: 112
class LightTexture {
public:
    std::shared_ptr<mce::Image>         mBrightnessImage;     // off+0
    const mce::DynamicTexture           mBrightnessTextureId; // off+16
    std::shared_ptr<mce::ClientTexture> mBrightnessTexture;   // off+24
    bool                                mIsTextureDirty;      // off+40
    BaseLightData                       mLightData;           // off+44

    SDK_API LightTexture(
        MinecraftGameplayGraphicsResources &minecraftGameplayGraphicsResources, const uint8_t subClientId
    );
#pragma SPHR_LINKER_SYM_ALIAS("??0LightTexture@@QEAA@AEAVMinecraftGameplayGraphicsResources@@E@Z", "?ctor@LightTexture@@QEAAPEAV1@AEAVMinecraftGameplayGraphicsResources@@E@Z")

    SDK_API LightTexture *ctor(
        MinecraftGameplayGraphicsResources &minecraftGameplayGraphicsResources, const uint8_t subClientId
    );
};