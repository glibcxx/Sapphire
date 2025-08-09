#pragma once

#include "../BaseActorRenderer.h"
#include "SDK/api/src-deps/MinecraftRenderer/Renderer/TexturePtr.h"
#include "SDK/api/src/common/world/phys/AABB.h"
#include "SDK/api/src-client/common/client/renderer/BaseActorRenderContext.h"

class ActorRenderData;
class Model;
namespace mce {
    class TextureGroup;
}

// size: 312
class ActorRenderer : public BaseActorRenderer {
public:
    uint64_t               mUnk120[2];                       // off+120
    bool                   mRenderingInventory;              // off+136
    const bool             mHasWaterHole;                    // off+137
    float                  mMinimumVRRenderDistance;         // off+140
    mce::MaterialPtr       mEntityAlphatestMaterial;         // off+144
    mce::MaterialPtr       mEntityAlphatestGlintMaterial;    // off+160
    mce::MaterialPtr       mModEntityAlphatestGlintMaterial; // off+176
    mce::TexturePtr        mGlintTexture;                    // off+192
    mce::TexturePtr        mAtlasTexture;                    // off+224
    mce::TextureGroup     &mTextureGroup;                    // off+248
    AABB                   mBaseRenderBounds;                // off+256
    std::shared_ptr<Model> mModel;                           // off+280
    mce::MaterialPtr       mLeashMat;                        // off+296

    // vtb+0
    virtual ~ActorRenderer() = default;

    // vtb+2
    virtual void render(BaseActorRenderContext &context, ActorRenderData &renderData) = 0;
};