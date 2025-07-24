#pragma once

#include "SDK/core/ApiManager.h"

#include "BaseActorRenderer.h"
#include "BaseActorRenderContext.h"
#include "BlockActorRenderData.h"

class BlockActorRenderer : public BaseActorRenderer {
public:
    BlockActorRenderer() = delete;

    // vtb+0
    virtual ~BlockActorRenderer() = 0;

    // vtb+2
    virtual void render(BaseActorRenderContext &context, BlockActorRenderData &renderData) = 0;

    // vtb+3
    virtual void unk_virtual_3() = 0;

    // vtb+4
    virtual std::vector<class NameTagRenderObject> extractText(
        class Tessellator            &tessellator,
        class BlockActor             &entity,
        const std::string            &str,
        const class std::vector<int> &widths,
        Vec3                          camTargetPos,
        bool                          realityFullVRMode
    ) = 0;
};