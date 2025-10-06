#pragma once

#include "SDK/core/ApiManager.h"
#include "SDK/api/src-deps/Core/Math/Color.h"
#include "SDK/api/src-client/common/client/renderer/MaterialPtr.h"
#include "SDK/api/src/common/world/phys/Vec2.h"
#include "SDK/api/src/common/world/phys/Vec4.h"
#include "SDK/api/src/common/CommonTypes.h"

class Actor;
class ScreenContext;
class BaseActorRenderContext;

// size: 56
class ActorShaderManager {
public:
    mce::MaterialPtr mEntityMaterial;            // off+8
    mce::MaterialPtr mTransparentEntityMaterial; // off+24
    mce::MaterialPtr mStaticMaterial;            // off+40

    // vtb+0
    virtual ~ActorShaderManager() = default;

    // vtb+1
    virtual mce::Color _getOverlayColor(Actor &entity, float a) const {
        return getOverlayColor(entity, a);
    }

    SDK_API static mce::Color getOverlayColor(Actor &entity, float a);

    SDK_API mce::MaterialPtr &getEntityMaterial();

    SDK_API mce::MaterialPtr &getTransparentEntityMaterial();

    SDK_API static void setupShaderParameters(
        ScreenContext          &screenContext,
        BaseActorRenderContext &entityContext,
        Actor                  &entity,
        const mce::Color       &overlay,
        const mce::Color       &changeColor,
        const mce::Color       &changeColor2,
        const mce::Color       &glintColor,
        float                   uvOffset1,
        float                   uvOffset2,
        float                   uvRot1,
        float                   uvRot2,
        const Vec2             &glintUVScale,
        const Vec4             &uvAnim,
        float                   br,
#if MC_VERSION <= v1_21_50
        Brightness lightEmission
#else
        uint8_t lightEmission
#endif
    );
};