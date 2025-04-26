#pragma once

#include "MovingBlockActor.h"

class PistonBlockActor : public BlockActor {
public:
    enum class PistonState : char {
        Retracted = 0,
        Expanding = 1,
        Expanded = 2,
        Retracting = 3,
    };

    bool  mSticky;
    float mProgress;
    float mLastProgress;
    bool  mWasPushedBackwardByANonStickyPiston;
    bool  mWasPulledForwardByAStickyPiston;

    PistonState mOldState;
    PistonState mState;
    PistonState mNewState;

    bool                                 mVerifyArm;
    bool                                 mShouldVerifyArmType;
    std::vector<BlockPos>                mAttachedBlocks;
    std::optional<std::vector<BlockPos>> mFutureClientAttachedBlocks;
    std::vector<BlockPos>                mBreakBlocks;

    float getProgress(float a) {
        if (a > 1.0f) a = 1.0f;
        return this->mLastProgress + (this->mProgress - this->mLastProgress) * a;
    }

    SDK_API void tick(class BlockSource &region);
};