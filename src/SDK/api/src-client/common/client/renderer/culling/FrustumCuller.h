#pragma once

#include "SDK/api/src-deps/Core/Math/Frustum.h"
#include "SDK/api/src/common/world/phys/Vec3.h"

// size: 204
class FrustumCuller {
public:
    Frustum mFrustumData; // off+0
    Vec3    mOff;         // off+192
};
static_assert(sizeof(FrustumCuller) == 204);
