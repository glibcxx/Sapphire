#include "LevelRenderer.h"

LevelRenderer *LevelRenderer::ctor(
    IClientInstance                                                 &clientInstance,
    MultiPlayerLevel                                                &level,
    std::shared_ptr<Options>                                         options,
    std::shared_ptr<mce::TextureGroup>                               textures,
    const Bedrock::NotNullNonOwnerPtr<const TextureAtlas>           &terrainTexture,
    WeakEntityRef                                                    localPlayer,
    HolographicPlatform                                             &holographicPlatform,
    GameRenderer                                                    &gameRenderer,
    const Bedrock::NotNullNonOwnerPtr<SoundPlayerInterface>         &soundPlayer,
    const Bedrock::NotNullNonOwnerPtr<GeometryGroup>                &geometryGroup,
    const SoundMapping                                              &sounds,
    const Bedrock::NotNullNonOwnerPtr<ActorResourceDefinitionGroup> &entityResourceDefGroup
) {
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        "\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x49\x8B\xC1\x48\x89\x45\x00\x4D\x8B\xF8"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x49\x8B\xC1\x48\x89\x45"_sig,
#endif
        &LevelRenderer::ctor>;
    return (this->*Bind::origin)(clientInstance, level, options, textures, terrainTexture, std::move(localPlayer), holographicPlatform, gameRenderer, soundPlayer, geometryGroup, sounds, entityResourceDefGroup);
}

void LevelRenderer::dtor() noexcept {
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\xD9\x48\x8D\x05"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x48\x8B\xF1\x48\x8D\x05"_sig,
#endif
        &LevelRenderer::dtor>;
    return (this->*Bind::origin)();
}

void LevelRenderer::renderLevel(ScreenContext &screenCtx, const FrameRenderObject &frameRenderObj) {
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x45\x32\xE4\x48\x8B\x07"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xC6\x44\x24\x00\x00\x48\x8B\x86\x00\x00\x00\x00\x48\x85\xC0"_sig,
#endif
        &LevelRenderer::renderLevel>;
    (this->*Bind::origin)(screenCtx, frameRenderObj);
}

void LevelRenderer::preRenderUpdate(ScreenContext &screenCtx, LevelRenderPreRenderUpdateParameters &params) {
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xF3\x41\x0F\x10\x4D\x00\x48\x8B\xCE"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xF3\x41\x0F\x10\x4C\x24\x00\x48\x8B\xCE"_sig,
#endif
        &LevelRenderer::preRenderUpdate>;
    (this->*Bind::origin)(screenCtx, params);
}
