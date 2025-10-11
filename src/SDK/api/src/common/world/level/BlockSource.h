#pragma once

#include "IConstBlockSource.h"

class ActorBlockSyncMessage;

class BlockSource : public IConstBlockSource, public std::enable_shared_from_this<BlockSource> {
public:
    const std::thread::id mOwnerThreadID;
    const bool            mAllowUnpopulatedChunks;
    const bool            mPublicSource;
    bool                  mCheckValidity;
    class Level          &mLevel;

    SDK_API bool setBlock(
        const BlockPos              &pos,
        const Block                 &block,
        int                          updateFlags,
        std::shared_ptr<BlockActor>  blockEntity,
        const ActorBlockSyncMessage *syncMsg
    );
};