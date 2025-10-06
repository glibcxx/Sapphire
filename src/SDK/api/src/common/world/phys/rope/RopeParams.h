#pragma once

#include "SDK/api/src-deps/Platform/Threading/Mutex.h"
#include "../Vec3.h"

// size: 88
struct RopeParams {
    enum Flags {
        None = 0,
        DynamicResize = 2,
        DynamicStretch = 4,
        CollisionEnabled = 8,
    };

    float  mNodeDist;         // off+0
    float  mNodeSize;         // off+4
    float  mGravity;          // off+8
    float  mSlack;            // off+12
    float  mMaxTension;       // off+16
    float  mVelDamping;       // off+20
    float  mRelaxation;       // off+24
    float  mFriction;         // off+28
    Vec3   mStartPin;         // off+32
    Vec3   mEndPin;           // off+44
    size_t mIterations;       // off+56
    size_t mDestroyDelay;     // off+64
    int    mFlags;            // off+72
    float  mLength;           // off+76
    float  mOriginalNodeDist; // off+80
};

// size: 40
struct RopeWave {
    enum class Direction : int32_t {
        StartToEnd = 0,
        EndToStart = 1,
    };

    Vec3      mForce;         // off+0
    float     mSpeed;         // off+12
    float     mDamping;       // off+16
    uint64_t  mCurNode;       // off+24
    float     mDistAlongNode; // off+32
    Direction mDir;           // off+36
};

// size: 24
struct RopePoint {
    Vec3 mOldPos;   // off+0
    Vec3 mToNewPos; // off+12
};

// size: 32
struct RopePoints {
    size_t                 mSize;   // off+0
    std::vector<RopePoint> mPoints; // off+8
};

// size: 16
struct RopePointsRef {
    const RopePoints          &mPoints;     // off+0
    Bedrock::Threading::Mutex &mPointMutex; // off+8
};
