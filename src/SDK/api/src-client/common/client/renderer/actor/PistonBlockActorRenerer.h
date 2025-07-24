#pragma once

#include "SDK/core/ApiManager.h"
#include "BlockActorRenderer.h"

class MovingBlockActorRenderer : public BlockActorRenderer {
public:
    // vtb+2
    SDK_API virtual void render(BaseActorRenderContext &context, BlockActorRenderData &renderData) override;
    MARK_VIRTUAL_HOOKABLE(&MovingBlockActorRenderer::render)
};

class PistonBlockActorRenderer : public BlockActorRenderer {
public:
    // vtb+2
    SDK_API virtual void render(BaseActorRenderContext &context, BlockActorRenderData &renderData) override;
    MARK_VIRTUAL_HOOKABLE(&PistonBlockActorRenderer::render)
};
