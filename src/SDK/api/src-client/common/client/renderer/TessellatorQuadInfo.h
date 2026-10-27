#pragma once

#include "SDK/api/src/common/world/phys/Vec3.h"

// size: 12
struct RenderChunkQuadInfo {
public:
    unsigned index;    // off+0
    unsigned reverse;  // off+0
    Vec3     centroid; // off+4
};
