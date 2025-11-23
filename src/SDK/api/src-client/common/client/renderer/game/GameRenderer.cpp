#include "GameRenderer.h"

std::weak_ptr<PlayerRenderView> GameRenderer::addPlayerRenderView(LevelRenderer &renderer) {
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xFA\x48\x8B\xD9\x33\xED"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x48\x8B\xF2\x48\x8B\xF9\x48\x89\x54\x24\x00\x00\x8B"_sig,
#endif
        &GameRenderer::addPlayerRenderView>;
    return (this->*Bind::origin)(renderer);
}
