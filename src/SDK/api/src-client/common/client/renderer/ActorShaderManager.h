#pragma once

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

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x55\x56\x57\x48\x83\xEC\x00\x8B\x82")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\xF3\x0F\x11\x54\x24")
    SDK_API static mce::Color getOverlayColor(Actor &entity, float a);

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x49\x8D\x55\x00\x4C\x8D\x4C\x24")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x4C\x8B\xF1\x33\xD2")
    SDK_API mce::MaterialPtr &getEntityMaterial();

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x33\xD2\x48\x8D\x79\x00\x48\x8B\x07\x48\x85\xC0\x74\x00\x48\x39\x50\x00\x74\x00\x32\xC0\xEB\x00\xB0\x00\x84\xC0\x0F\x84\x00\x00\x00\x00\x48\x89\x54\x24")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x4C\x8B\xF1\x33\xC9\x49\x8B\x46\x00\x48\x85\xC0\x74\x00\x48\x39\x48\x00\x74\x00\x32\xC0\xEB\x00\xB0\x00\x84\xC0\x0F\x84\x00\x00\x00\x00\x48\x89\x4C\x24")
    SDK_API mce::MaterialPtr &getTransparentEntityMaterial();

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x4C\x8B\xA4\x24\x00\x00\x00\x00\x4C\x8B\xBC\x24")
    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x4C\x8B\x75\x00\xBF\x00\x00\x00\x00\x48\x85\xF6")
    SPHR_DECL_API("1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x8B\xBC\x24\x00\x00\x00\x00\x48\x81\xC4\x00\x00\x00\x00\x41\x5E\x00\x00\x00\x00\x49\x8B\x85")
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