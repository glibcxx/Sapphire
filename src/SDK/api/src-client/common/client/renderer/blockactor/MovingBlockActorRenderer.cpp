#include "MovingBlockActorRenderer.h"

MovingBlockActorRenderer *MovingBlockActorRenderer::ctor(
    std::shared_ptr<mce::TextureGroup> textures,
    BlockTessellator                  &blockTessellator
) {
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x49\x8B\xD8\x00\x8B\xFA"_sig,
        &MovingBlockActorRenderer::ctor>;
    return (this->*Hook::origin)(std::move(textures), blockTessellator);
}

void MovingBlockActorRenderer::render(BaseActorRenderContext &context, BlockActorRenderData &renderData) {
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\xB4\x24\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x00\x8B\x00\x48\x89\x55\x00\x4C\x8B\x00\x33\xDB"_sig,
        &MovingBlockActorRenderer::render,
        SPHR_FUNCDNAME>;
    return (this->*Hook::origin)(context, renderData);
}
