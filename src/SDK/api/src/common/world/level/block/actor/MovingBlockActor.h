#pragma once

#include "BlockActor.h"
#include "SDK/api/src/common/world/level/IConstBlockSource.h"

class MovingBlockActor : public BlockActor {
public:
    const Block                *mBlock;
    const Block                *mExtraBlock;
    std::shared_ptr<BlockActor> mBlockEntity;
    BlockPos                    mPistonPos;
    bool                        mPistonBlockExpanding;
    AABB                        mCollisionShape;
    bool                        mPreserved;
    int                         mPreservedLifespan;

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xDA\x0F\x29\x7C\x24")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x00\x00\x00\x00\x00\x00\x00\x49\x8B\xF0\x0F\x29\x7C\x24")
    SDK_API Vec3 getDrawPos(const IConstBlockSource &region, float alpha) const;

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x44\x8B\x45\x00\x48\x83\xC3\x00\xE9\x00\x00\x00\x00\xF3\x0F\x10\x8F")
    SDK_API void moveCollidedEntities(PistonBlockActor &pistonBlock, BlockSource &region);
};
