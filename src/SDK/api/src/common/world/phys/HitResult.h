#pragma once

#include "SDK/api/src/common/CommonTypes.h"
#include "SDK/api/src/common/world/phys/Vec3.h"
#include "SDK/api/src/common/entity/WeakEntityRef.h"

enum HitResultType {
    TILE = 0,
    ENTITY = 1,
    ENTITY_OUT_OF_RANGE = 2,
    NO_HIT = 3,
};

// size: 112
class HitResult {
public:
    Vec3          mStartPos;     // off+0
    Vec3          mRayDir;       // off+12
    HitResultType mType;         // off+24
    FacingID      mFacing;       // off+28
    BlockPos      mBlock;        // off+32
    Vec3          mPos;          // off+44
    WeakEntityRef mEntity;       // off+56
    bool          mIsHitLiquid;  // off+80
    FacingID      mLiquidFacing; // off+81
    BlockPos      mLiquid;       // off+84
    Vec3          mLiquidPos;    // off+96
    bool          mIndirectHit;  // off+108
};