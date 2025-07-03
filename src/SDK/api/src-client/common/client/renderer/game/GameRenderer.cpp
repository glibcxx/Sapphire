#include "GameRenderer.h"

std::weak_ptr<PlayerRenderView> GameRenderer::addPlayerRenderView(LevelRenderer &renderer) {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_50
        "\xE8\x00\x00\x00\x00\x48\x8B\x08\x48\x8B\x50\x00\x45\x33\xE4\x4C\x89\x20\x4C\x89\x60\x00\x48\x89\x8F"_sig,
#endif
        &GameRenderer::addPlayerRenderView,
        sapphire::deRefCall>;
    return (this->*Hook::origin)(renderer);
}
