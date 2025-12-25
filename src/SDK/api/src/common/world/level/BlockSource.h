#pragma once

#include "IConstBlockSource.h"

struct ActorBlockSyncMessage;
class Level;

class BlockSource : public IConstBlockSource, public std::enable_shared_from_this<BlockSource> {
public:
    const std::thread::id mOwnerThreadID;          // off+24
    const bool            mAllowUnpopulatedChunks; // off+28
    const bool            mPublicSource;           // off+29
    bool                  mCheckValidity;          // off+30
    Level                &mLevel;                  // off+32

    BlockSource() = delete;
    BlockSource(const BlockSource &) = delete;

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x45\x8B\xE9")
    SDK_API bool setBlock(
        const BlockPos              &pos,
        const Block                 &block,
        int                          updateFlags,
        std::shared_ptr<BlockActor>  blockEntity,
        const ActorBlockSyncMessage *syncMsg
    );
};