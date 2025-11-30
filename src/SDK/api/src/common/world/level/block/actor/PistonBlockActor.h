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

    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "\x40\x53\x48\x83\xEC\x00\x45\x0F\xB6\xD0")
    SDK_API PistonBlockActor *ctor(const BlockPos &pos, bool isSticky);

    // vtb+1
    SAPPHIRE_API("1.21.2", "\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\xB4\x24\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x4C\x89\x4D")
    SAPPHIRE_API("1.21.50,1.21.60", "\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x49\x8B\xF1\x4C\x89\x4D\x00\x49\x8B\xF8\x4C\x89\x45\x00\x48\x8B\xDA")
    SDK_API virtual void load(Level &level, const CompoundTag &tag, DataLoadHelper &dataLoadHelper) override;

    // vtb+7
    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "\x48\x8B\xC4\x48\x89\x58\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x44\x0F\x29\x40\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4C\x8B\xF2\x48\x8B\xF1")
    SDK_API virtual void tick(BlockSource &region) override;

    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "\x40\x53\x56\x57\x41\x56\x41\x57\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x0F\xB6\xB9")
    SDK_API /*virtual*/ void _onUpdatePacket(const CompoundTag &data, BlockSource &region) /*override*/;

    SAPPHIRE_API("1.21.2", "\x40\x55\x56\x41\x55\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05")
    SAPPHIRE_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x48\x8B\x02\x4C\x8B\xF2\x4C\x8B\xE1")
    SDK_API void _spawnBlocks(BlockSource &region);

    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x83\xC3\x0C\x48\x3B\xDF\x75\x00\x48\x8B\x4C\x24")
    SDK_API void _spawnMovingBlock(BlockSource &region, const BlockPos &blockPos);

    float getProgress(float a) {
        if (a > 1.0f) a = 1.0f;
        return this->mLastProgress + (this->mProgress - this->mLastProgress) * a;
    }

    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "\x40\x53\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x48\x8B\x02\x4C\x8B\xC2")
    SDK_API void _sortAttachedBlocks(BlockSource &region);
};