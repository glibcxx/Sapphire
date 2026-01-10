#pragma once

#include "MaterialType.h"

// size: 12
class Material {
public:
    MaterialType mType;                // off+0
    bool         mNeverBuildable;      // off+4
    bool         mLiquid;              // off+5
    bool         mBlocksMotion;        // off+6
    bool         mBlocksPrecipitation; // off+7
    bool         mSolid;               // off+8
    bool         mSuperHot;            // off+9
};