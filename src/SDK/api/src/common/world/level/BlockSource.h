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

    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x45\x8B\xE9")
    SDK_API bool setBlock(
        const BlockPos              &pos,
        const Block                 &block,
        int                          updateFlags,
        std::shared_ptr<BlockActor>  blockEntity,
        const ActorBlockSyncMessage *syncMsg
    );
};