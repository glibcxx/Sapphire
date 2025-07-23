#pragma once

#include "SDK/core/ApiManager.h"
#include "SDK/api/src-deps/Core/Math/Color.h"

class Actor;

class ActorShaderManager {
public:
    // vtb+0
    virtual ~ActorShaderManager() = 0;

    // vtb+1
    SDK_VAPI mce::Color &_getOverlayColor(Actor &entity, float a) const;
};