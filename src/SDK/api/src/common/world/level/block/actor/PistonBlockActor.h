#pragma once

#include "MovingBlockActor.h"

class BlockSource;
class CompoundTag;

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

    SDK_API PistonBlockActor(const BlockPos &pos, bool isSticky);
#pragma SPHR_LINKER_SYM_ALIAS("??0PistonBlockActor@@QEAA@AEBVBlockPos@@_N@Z", "?ctor@PistonBlockActor@@QEAAPEAV1@AEBVBlockPos@@_N@Z")

    SDK_API PistonBlockActor *ctor(const BlockPos &pos, bool isSticky);

    SDK_API virtual void load(Level &level, const CompoundTag &tag, DataLoadHelper &dataLoadHelper) override;

    // vtb+7
    SDK_API virtual void tick(BlockSource &region) override;

    SDK_API /*virtual*/ void _onUpdatePacket(const CompoundTag &data, BlockSource &region) /*override*/;

    SDK_API void _spawnBlocks(BlockSource &region);

    SDK_API void _spawnMovingBlock(BlockSource &region, const BlockPos &blockPos);

    float getProgress(float a) {
        if (a > 1.0f) a = 1.0f;
        return this->mLastProgress + (this->mProgress - this->mLastProgress) * a;
    }

    SDK_API void _sortAttachedBlocks(BlockSource &region);
};