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

    SDK_API Vec3 getDrawPos(const IConstBlockSource &region, float alpha) const;

    SDK_API void moveCollidedEntities(PistonBlockActor &pistonBlock, BlockSource &region);
};
