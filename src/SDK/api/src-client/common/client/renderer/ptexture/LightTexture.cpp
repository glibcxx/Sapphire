#include "LightTexture.h"

LightTexture *LightTexture::ctor(
    MinecraftGameplayGraphicsResources &minecraftGameplayGraphicsResources,
    const uint8_t                       subClientId
) {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x56\x41\x57\x48\x83\xEC\x00\x4C\x8B\xF2\x48\x8B\xF9\x48\x89\x4C\x24\x00\x45\x33\xFF"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x56\x41\x57\x48\x83\xEC\x00\x48\x8B\xEA\x48\x8B\x00\x48\x89\x4C\x24\x00\x45\x33\xFF\x4C\x89\x39"_sig,
#endif
        &LightTexture::ctor>;
    return (this->*Hook::origin)(minecraftGameplayGraphicsResources, subClientId);
}