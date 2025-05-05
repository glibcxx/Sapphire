#pragma once

#include <array>

#include "SDK/api/src/common/world/phys/Vec3.h"
#include "SDK/api/src/common/world/level/BlockPos.h"

struct WireframeQuad {
    std::array<Vec3, 4> mQuad;
    int                 mColor;
};

struct Wireframe {
    BlockPos mPos;
    BlockPos mBox;

    std::array<WireframeQuad, 24> mQuadList;
};
