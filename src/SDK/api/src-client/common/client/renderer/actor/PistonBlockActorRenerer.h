#pragma once

#include "SDK/core/Core.h"
#include "BlockActorRenderer.h"

class MovingBlockActorRenderer : public BlockActorRenderer {
public:
    // vtb+2
    SDK_VAPI void render(BaseActorRenderContext &context, BlockActorRenderData &renderData);
};

class PistonBlockActorRenderer : public BlockActorRenderer {
public:
    // vtb+2
    SDK_VAPI void render(BaseActorRenderContext &context, BlockActorRenderData &renderData);
};
