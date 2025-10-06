#include "ActorShaderManager.h"

mce::Color ActorShaderManager::getOverlayColor(Actor &entity, float a) {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x48\x83\xEC\x00\x8B\x82"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\xF3\x0F\x11\x54\x24"_sig,
#endif
        &ActorShaderManager::getOverlayColor>;
    return Hook::origin(entity, a);
}

mce::MaterialPtr &ActorShaderManager::getEntityMaterial() {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x49\x8D\x55\x00\x4C\x8D\x4C\x24"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x4C\x8B\xF1\x33\xD2"_sig,
#endif
        &ActorShaderManager::getEntityMaterial>;
    return (this->*Hook::origin)();
}

mce::MaterialPtr &ActorShaderManager::getTransparentEntityMaterial() {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x33\xD2\x48\x8D\x79\x00\x48\x8B\x07\x48\x85\xC0\x74\x00\x48\x39\x50\x00\x74\x00\x32\xC0\xEB\x00\xB0\x00\x84\xC0\x0F\x84\x00\x00\x00\x00\x48\x89\x54\x24"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x4C\x8B\xF1\x33\xC9\x49\x8B\x46\x00\x48\x85\xC0\x74\x00\x48\x39\x48\x00\x74\x00\x32\xC0\xEB\x00\xB0\x00\x84\xC0\x0F\x84\x00\x00\x00\x00\x48\x89\x4C\x24"_sig,
#endif
        &ActorShaderManager::getEntityMaterial>;
    return (this->*Hook::origin)();
}

void ActorShaderManager::setupShaderParameters(
    ScreenContext          &screenContext,
    BaseActorRenderContext &entityContext,
    Actor                  &entity,
    const mce::Color       &overlay,
    const mce::Color       &changeColor,
    const mce::Color       &changeColor2,
    const mce::Color       &glintColor,
    float                   uvOffset1,
    float                   uvOffset2,
    float                   uvRot1,
    float                   uvRot2,
    const Vec2             &glintUVScale,
    const Vec4             &uvAnim,
    float                   br,
#if MC_VERSION <= v1_21_50
    Brightness lightEmission
#else
    uint8_t lightEmission
#endif
) {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x4C\x8B\xA4\x24\x00\x00\x00\x00\x4C\x8B\xBC\x24"_sig,
#elif MC_VERSION == v1_21_50
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x4C\x8B\x75\x00\xBF\x00\x00\x00\x00\x48\x85\xF6"_sig,
#elif MC_VERSION == v1_21_60
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\xBC\x24\x00\x00\x00\x00\x48\x81\xC4\x00\x00\x00\x00\x41\x5E\x00\x00\x00\x00\x49\x8B\x85"_sig,
#endif
        &ActorShaderManager::setupShaderParameters>;
    Hook::origin(screenContext, entityContext, entity, overlay, changeColor, changeColor2, glintColor, uvOffset1, uvOffset2, uvRot1, uvRot2, glintUVScale, uvAnim, br, lightEmission);
}