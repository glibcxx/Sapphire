#pragma once

#include "SDK/api/src-deps/Core/Math/Color.h"
#include "SDK/api/src/common/world/level/dimension/DimensionType.h"

// size: 64
class BaseLightData {
public:
    mce::Color    mSunriseColor;              // off+0
    float         mGamma;                     // off+16
    float         mSkyDarken;                 // off+20
    DimensionType mDimensionType;             // off+24
    float         mDarkenWorldAmount;         // off+28
    float         mPreviousDarkenWorldAmount; // off+32
    bool          mNightvisionActive;         // off+36
    float         mNightvisionScale;          // off+40
    bool          mUnderwaterVision;          // off+44
    float         mUnderwaterScale;           // off+48
    int           mSkyFlashTime;              // off+52
    uint32_t      mUnk56;                     // off+56
    uint32_t      mUnk60;                     // off+60
};