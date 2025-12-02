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

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x56\x41\x57\x48\x83\xEC\x00\x4C\x8B\xF2\x48\x8B\xF9\x48\x89\x4C\x24\x00\x45\x33\xFF")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x56\x41\x57\x48\x83\xEC\x00\x48\x8B\xEA\x48\x8B\x00\x48\x89\x4C\x24\x00\x45\x33\xFF\x4C\x89\x39")
    SDK_API LightTexture *ctor(
        MinecraftGameplayGraphicsResources &minecraftGameplayGraphicsResources, const uint8_t subClientId
    );
};