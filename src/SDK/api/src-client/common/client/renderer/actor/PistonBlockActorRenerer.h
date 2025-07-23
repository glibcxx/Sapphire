#pragma once

#include "SDK/core/ApiManager.h"
#include "BlockActorRenderer.h"

class MovingBlockActorRenderer : public BlockActorRenderer {
public:
    // vtb+2
    SDK_VAPI void render(BaseActorRenderContext &context, BlockActorRenderData &renderData) override;
};

class PistonBlockActorRenderer : public BlockActorRenderer {
public:
    // vtb+2
    SDK_VAPI void render(BaseActorRenderContext &context, BlockActorRenderData &renderData) override;
};
