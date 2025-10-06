#pragma once

#include "SDK/api/src/common/world/phys/Vec3.h"

class Geometry {
public:
    // size: 12
    struct NodeVertex {
        uint32_t mPositionIndex; // off+0
        uint32_t mNormalIndex;   // off+4
        uint32_t mUVIndex;       // off+8
    };

    // size: 128
    struct NodePolyMesh {
        std::vector<Vec3>                      mPositions;        // off+0
        std::vector<Vec3>                      mNormals;          // off+24
        std::vector<Vec2>                      mUVs;              // off+48
        std::vector<std::array<NodeVertex, 3>> mTris;             // off+72
        std::vector<std::array<NodeVertex, 4>> mQuads;            // off+96
        bool                                   mAreUVsNormalized; // off+120
    };
};