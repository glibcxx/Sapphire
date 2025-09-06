#pragma once

#include "BlockActorRenderer.h"

class PistonBlockActorRenderer : public BlockActorRenderer {
public:
    // vtb+2
    SDK_API virtual void render(BaseActorRenderContext &context, BlockActorRenderData &renderData) override;
};
