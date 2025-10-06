#pragma once

#include "../BaseActorRenderer.h"
#include "SDK/api/src-deps/MinecraftRenderer/Renderer/TexturePtr.h"
#include "SDK/api/src/common/world/phys/AABB.h"
#include "SDK/api/src-client/common/client/renderer/BaseActorRenderContext.h"

class ActorRenderData;
class Model;
class RenderParams;
struct RopePoints;
namespace mce {
    class TextureGroup;
}

// size: 312
class ActorRenderer : public BaseActorRenderer {
public:
    std::weak_ptr<void>                mUnk120;                       // off+120
    bool                               mRenderingInventory;           // off+136
    const bool                         mHasWaterHole;                 // off+137
    float                              mMinimumVRRenderDistance;      // off+140
    mce::MaterialPtr                   mEntityAlphatestMaterial;      // off+144
    mce::MaterialPtr                   mEntityAlphatestGlintMaterial; // off+160
    mce::TexturePtr                    mGlintTexture;                 // off+176
    mce::TexturePtr                    mAtlasTexture;                 // off+208
    std::shared_ptr<mce::TextureGroup> mTextureGroup;                 // off+240
    AABB                               mBaseRenderBounds;             // off+256
    std::shared_ptr<Model>             mModel;                        // off+280
    mce::MaterialPtr                   mLeashMat;                     // off+296

    static constexpr Vec2 DEFAULT_RENDER_BOUNDS{1.0f, 1.0f};

    SDK_API ActorRenderer(
        std::shared_ptr<Model>             model,
        std::shared_ptr<mce::TextureGroup> textures,
        const Vec2                        &renderDim,
        const Vec3                        &renderDimOffset,
        const bool                         hasWaterHole
    );
#pragma SPHR_LINKER_SYM_ALIAS(                                                                                  \
    "??0ActorRenderer@@QEAA@V?$shared_ptr@VModel@@@std@@AEAVTextureGroup@mce@@AEBVVec2@@AEBVVec3@@_N@Z",        \
    "?ctor@ActorRenderer@@QEAAPEAV1@V?$shared_ptr@VModel@@@std@@AEAVTextureGroup@mce@@AEBVVec2@@AEBVVec3@@_N@Z" \
)

    SDK_API ActorRenderer(
        std::shared_ptr<mce::TextureGroup> textures, const Vec2 &renderDim, const Vec3 &renderDimOffset, const bool hasWaterHole
    );
#pragma SPHR_LINKER_SYM_ALIAS(                                                      \
    "??0ActorRenderer@@QEAA@AEAVTextureGroup@mce@@AEBVVec2@@AEBVVec3@@_N@Z",        \
    "?ctor@ActorRenderer@@QEAAPEAV1@AEAVTextureGroup@mce@@AEBVVec2@@AEBVVec3@@_N@Z" \
)

    SDK_API ActorRenderer *ctor(
        std::shared_ptr<Model>             model,
        std::shared_ptr<mce::TextureGroup> textures,
        const Vec2                        &renderDim,
        const Vec3                        &renderDimOffset,
        const bool                         hasWaterHole
    );

    SDK_API ActorRenderer *ctor(
        std::shared_ptr<mce::TextureGroup> textures, const Vec2 &renderDim, const Vec3 &renderDimOffset, const bool hasWaterHole
    );

    // vtb+0
    virtual ~ActorRenderer() = default;

    // vtb+2
    virtual void render(BaseActorRenderContext &context, ActorRenderData &renderData) = 0;

    // vtb+3
    virtual void renderDebug(BaseActorRenderContext &renderContext, ActorRenderData &entityRenderData);

    // vtb+4
    virtual void renderEffects(BaseActorRenderContext &renderContext, ActorRenderData &entityRenderData);

    // vtb+5
    virtual void renderFlame(BaseActorRenderContext &renderContext, ActorRenderData &entityRenderData);

    // vtb+6
    virtual void renderLeash(BaseActorRenderContext &renderContext, ActorRenderData &entityRenderData);

    // vtb+7
    virtual void renderWaterHole(BaseActorRenderContext &renderContext, ActorRenderData &entityRenderData);

    // vtb+8
    virtual void addAdditionalRenderingIfNeeded(std::shared_ptr<mce::TextureGroup> textures);

    // vtb+9
    virtual AABB getRenderBounds(const Actor &entity) const;

    // vtb+10
    virtual Vec3 getLeashOffset(Actor &actor, float yRot, float yRotPrev, float a, bool isBaby, bool legacyOffset);

    // vtb+11
    virtual void setIsOnScreen(Actor &actor, const bool isOnScreen, float distance) const;

    // vtb+12
    virtual bool shouldUpdateBonesAndEffectsIfOffScreen(RenderParams &renderParams) const;

    // vtb+13
    virtual bool shouldUpdateEffectsIfOffScreen(RenderParams &renderParams) const;

    SDK_API void _drawRopeRange(
        BaseActorRenderContext &renderContext,
        int                     begin,
        int                     end,
        const Vec3             &basisX,
        const Vec3             &basisY,
        const Vec3             &deltaNorm,
        const RopePoints       &nodes,
        mce::TexturePtr        &tex,
        float                   a,
        Vec3                    startPoint,
        Vec3                    endPoint
    );
};