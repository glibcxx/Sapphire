#pragma once

#include "BlockActor.h"
#include "SDK/api/src/common/world/level/IConstBlockSource.h"

class MovingBlockActor : public BlockActor {
public:
    const struct Block                *mBlock;
    const struct Block                *mExtraBlock;
    std::shared_ptr<struct BlockActor> mBlockEntity;
    BlockPos                           mPistonPos;
    bool                               mPistonBlockExpanding;
    AABB                               mCollisionShape;
    bool                               mPreserved;
    int                                mPreservedLifespan;

    SDK_API Vec3 getDrawPos(const IConstBlockSource &region, float alpha) const;
    MARK_HOOKABLE(&MovingBlockActor::getDrawPos)
};
