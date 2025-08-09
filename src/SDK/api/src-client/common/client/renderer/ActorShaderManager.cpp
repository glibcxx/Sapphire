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