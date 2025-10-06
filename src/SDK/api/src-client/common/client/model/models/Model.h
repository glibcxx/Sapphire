#pragma once

#include "SDK/api/src/common/AppPlatformListener.h"
#include "SDK/api/src/common/world/phys/AABB.h"
#include "SDK/api/src-client/common/client/renderer/MaterialPtr.h"

class ModelPart;
class ParticleEffectPtr;
class ModelPartLocator;
class RenderController;
class ExpressionNode;
class DataDrivenGeometry;
class ScreenContext;
class BaseActorRenderContext;
class Actor;
class Mob;

// size: 280 (1.12.2/1.21.50), 296 (1.21.60)
class Model : public AppPlatformListener {
public:
    ;                                                                                 // off+(1.12.2/1.21.50), (1.21.60 = 1.21.50 + 16)
    Vec3                                                     mLeashOffset;            // off+16
    float                                                    mAttackTime;             // off+28
    bool                                                     mYoung;                  // off+32
    bool                                                     mRiding;                 // off+33
    MaterialVariants                                         mMaterialVariants;       // off+40
    std::vector<ModelPart *>                                 mAllParts;               // off+72
    std::unordered_map<HashedString, ParticleEffectPtr>      mParticleEffectsMap;     // off+96
    std::unordered_map<HashedString, ModelPartLocator>       mLocators;               // off+160
    std::vector<std::pair<RenderController, ExpressionNode>> mRenderControllers;      // off+224
    std::vector<std::unique_ptr<DataDrivenGeometry>>         mGeometries;             // off+248
    size_t                                                   mQueryableGeometryIndex; // off+272

    // vtb+0
    virtual ~Model();

    // vtb+20
    virtual void clear();

    // vtb+21
    virtual void preDraw(ScreenContext &);

    // vtb+22
    virtual void postDraw(ScreenContext &);

    // vtb+23
    virtual void render(
        BaseActorRenderContext &baseActorRenderContext,
        Actor                  &entity,
        float                   time,
        float                   r,
        float                   bob,
        float                   yRot,
        float                   xRot,
        float                   scale
    );

    // vtb+24
    virtual void render(BaseActorRenderContext &baseActorRenderContext);

    // vtb+25
    virtual void render(
        ScreenContext &screenContext,
        Actor         &entity,
        float          time,
        float          r,
        float          bob,
        float          yRot,
        float          xRot,
        float          scale
    );

    // vtb+26
    virtual void render(ScreenContext &screenContext);

    // vtb+27
    virtual void setupAnim();

    // vtb+28
    virtual void setupAnim(float time, float r, float bob, float yRot, float xRot, float scale);

    // vtb+29
    virtual void prepareMobModel(Mob &mob, float time, float r, float a);

    // vtb+30
    virtual Vec3 getLeashOffsetPosition(bool isBaby) const;

    // vtb+31
    virtual float getHeightAdjustment() const;

    // vtb+32
    virtual AABB buildAABB() const;
};