#pragma once

#include "SDK/core/ApiManager.h"
#include "SDK/api/src-deps/Core/Math/Color.h"
#include "SDK/api/src-client/common/client/renderer/MaterialPtr.h"

class Actor;

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
};