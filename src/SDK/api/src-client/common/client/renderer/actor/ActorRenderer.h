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

    SAPPHIRE_API("1.21.2", "\x48\x8B\xC4\x48\x89\x58\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x4D\x8B\xE9")
    SAPPHIRE_API("1.21.50,1.21.60", "\x48\x8B\xC4\x48\x89\x58\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x4C\x89\x4D")
    SDK_API ActorRenderer *ctor(
        std::shared_ptr<Model>             model,
        std::shared_ptr<mce::TextureGroup> textures,
        const Vec2                        &renderDim,
        const Vec3                        &renderDimOffset,
        const bool                         hasWaterHole
    );

    SAPPHIRE_API("1.21.2", "\x48\x8B\xC4\x48\x89\x58\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x4D\x8B\xE1")
    SAPPHIRE_API("1.21.50,1.21.60", "\x48\x8B\xC4\x48\x89\x58\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x4D\x8B\xE9")
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

    SAPPHIRE_API("1.21.2", "\x48\x8B\xC4\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x44\x0F\x29\x40\x00\x44\x0F\x29\x88\x00\x00\x00\x00\x44\x0F\x29\x90\x00\x00\x00\x00\x44\x0F\x29\x98\x00\x00\x00\x00\x44\x0F\x29\xA0\x00\x00\x00\x00\x44\x0F\x29\xA8\x00\x00\x00\x00\x44\x0F\x29\xB0\x00\x00\x00\x00\x44\x0F\x29\xB8\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x45\x8B\xE1")
    SAPPHIRE_API("1.21.50,1.21.60", "\x48\x8B\xC4\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x44\x0F\x29\x40\x00\x44\x0F\x29\x88\x00\x00\x00\x00\x44\x0F\x29\x90\x00\x00\x00\x00\x44\x0F\x29\x98\x00\x00\x00\x00\x44\x0F\x29\xA0\x00\x00\x00\x00\x44\x0F\x29\xA8\x00\x00\x00\x00\x44\x0F\x29\xB0\x00\x00\x00\x00\x44\x0F\x29\xB8\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x45\x8B\xE9")
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