#pragma once

#include "SDK/core/Core.h"

#include "BaseActorRenderer.h"
#include "BaseActorRenderContext.h"
#include "BlockActorRenderData.h"

class BlockActorRenderer : public BaseActorRenderer {
public:
    BlockActorRenderer() = delete;

    // vtb+0
    SDK_VAPI ~BlockActorRenderer();

    // vtb+2
    SDK_VAPI void render(BaseActorRenderContext &context, BlockActorRenderData &renderData) /* = 0 */;
};