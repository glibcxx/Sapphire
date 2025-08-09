#pragma once

#include "SDK/core/ApiManager.h"
#include "SDK/api/src/common/world/level/block/actor/BlockActor.h"
#include "SDK/api/src-client/common/client/renderer/MaterialPtr.h"
#include "SDK/api/src-deps/MinecraftRenderer/FrameBuilder/RenderMetadata.h"
#include "SDK/api/src-deps/MinecraftRenderer/Resources/TextureResourceService.h"
#include "../BaseActorRenderer.h"
#include "../BaseActorRenderContext.h"

class Block;
class BlockSource;

// size: 96 (1.21.2), 136 (1.21.50/1.21.60)
struct BlockActorRenderData {
    BlockSource                 &renderSource;        // off+0
    BlockActor                  &entity;              // off+8
    const Block                 &block;               // off+16
    const Vec3                  &position;            // off+24
    const mce::MaterialPtr      &forcedMat;           // off+32
    const mce::ClientTexture     forcedTex;           // off+40
    int                          breakingAmount;      // off+64
    const dragon::RenderMetadata actorRenderMetadata; // off+72
};

// size: 120
class BlockActorRenderer : public BaseActorRenderer {
public:
    // vtb+0
    virtual ~BlockActorRenderer() = default;

    // vtb+2
    virtual void render(BaseActorRenderContext &context, BlockActorRenderData &renderData) = 0;

    // vtb+3
    virtual void renderAlpha(BaseActorRenderContext &renderContext, BlockActorRenderData &blockEntityRenderData) {}

    // vtb+4
    virtual std::vector<NameTagRenderObject> extractText(
        class Tessellator            &tessellator,
        class BlockActor             &entity,
        const std::string            &str,
        const class std::vector<int> &widths,
        Vec3                          camTargetPos,
        bool                          realityFullVRMode
    );
};