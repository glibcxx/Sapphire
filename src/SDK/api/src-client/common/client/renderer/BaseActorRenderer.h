#pragma once

#include "ActorShaderManager.h"
#include "SDK/api/src/common/world/phys/Vec3.h"
#include "SDK/api/src-deps/MinecraftRenderer/Objects/NameTagRenderObjectCollection.h"

class Tessellator;

// size: 120
class BaseActorRenderer : public ActorShaderManager {
public:
    mce::MaterialPtr mNameTagMat;             // off+56
    mce::MaterialPtr mDepthTestedNameTagMat;  // off+72
    mce::MaterialPtr mDepthTestedNameTextMat; // off+88
    mce::MaterialPtr mDepthTestedHealthMat;   // off+104

    // vtb+0
    virtual ~BaseActorRenderer() = default;

    SDK_API std::vector<NameTagRenderObject> extractRenderTextObjects(
        Tessellator            &tessellator,
        const std::string      &str,
        const std::vector<int> &widths,
        const Vec3             &pos,
        const mce::Color       &color,
        bool                    realityFullVRMode
    );
    MARK_HOOKABLE(&BaseActorRenderer::extractRenderTextObjects)
};