#pragma once

#include "BlockActorRenderer.h"
#include "SDK/api/src-deps/MinecraftRenderer/renderer/TexturePtr.h"
#include "SDK/api/src-client/common/client/model/models/PistonArmModel.h"

// size: 3120 (1.21.2), 2904 (1.21.50), 2896 (1.21.60)
class PistonBlockActorRenderer : public BlockActorRenderer {
public:
    mce::TexturePtr mPistonArmTex;       // off+120
    mce::TexturePtr mPistonArmStickyTex; // off+152
    PistonArmModel  mPistonArm;          // off+184

    // vtb+2
    SDK_API virtual void render(BaseActorRenderContext &context, BlockActorRenderData &renderData) override;
};
