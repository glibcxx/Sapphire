#pragma once

#include "BlockActorRenderer.h"
#include "SDK/api/src-deps/MinecraftRenderer/Renderer/TexturePtr.h"

class BlockTessellator;
namespace mce {
    class TextureGroup;
}

// size: 384 (1.21.2), 400 (1.21.50/1.21.60)
class MovingBlockActorRenderer : public BlockActorRenderer {
public:
    BlockTessellator &mBlockTessellator; // off+120
    mce::TexturePtr   mAtlasTexture;     // off+128

    /*
        0  moving_block_double_side
        3  moving_block_blend
        4  moving_block
        6  moving_block_alpha
        7  moving_block_alpha
        8  moving_block_seasons
        9  moving_block_alpha_seasons
        10 moving_block_alpha_single_side
    */
#if MC_VERSION == v1_21_2
    mce::MaterialPtr mBlockMaterials[14]; // off+160
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    mce::MaterialPtr mBlockMaterials[15]; // off+160
#endif

    SDK_API MovingBlockActorRenderer(std::shared_ptr<mce::TextureGroup> textures, BlockTessellator &blockTessellator);
#pragma SPHR_LINKER_SYM_ALIAS(                                                                                  \
    "??0MovingBlockActorRenderer@@QEAA@V?$shared_ptr@VTextureGroup@mce@@@std@@AEAVBlockTessellator@@@Z",        \
    "?ctor@MovingBlockActorRenderer@@QEAAPEAV1@V?$shared_ptr@VTextureGroup@mce@@@std@@AEAVBlockTessellator@@@Z" \
)

    SDK_API MovingBlockActorRenderer *ctor(std::shared_ptr<mce::TextureGroup> textures, BlockTessellator &blockTessellator);

    // vtb+2
    SDK_API virtual void render(BaseActorRenderContext &context, BlockActorRenderData &renderData) override;
};
#if MC_VERSION == v1_21_2
static_assert(sizeof(MovingBlockActorRenderer) == 384);
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
static_assert(sizeof(MovingBlockActorRenderer) == 400);
#endif
