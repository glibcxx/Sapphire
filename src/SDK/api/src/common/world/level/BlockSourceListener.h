#pragma once

#include "SDK/api/src/common/world/level/BlockPos.h"

class Actor;
class ActorBlockSyncMessage;
class Block;
class BlockActor;
class BlockSource;

enum class BlockChangedEventTarget : uint8_t {
    SelfBlock = 0,
    NeighborBlock = 1,
};

class BlockSourceListener {
public:
    // vtb+0
    virtual ~BlockSourceListener() {}

    // vtb+1
    virtual void onSourceCreated(BlockSource &source) = 0;

    // vtb+2
    virtual void onSourceDestroyed(BlockSource &source) = 0;

    // vtb+3
    virtual void onAreaChanged(BlockSource &source, const BlockPos &min, const BlockPos &max) = 0;

    // vtb+4
    virtual void onBlockChanged(
        BlockSource                 &source,
        const BlockPos              &pos,
        uint32_t                     layer,
        const Block                 &block,
        const Block                 &oldBlock,
        int32_t                      updateFlags,
        const ActorBlockSyncMessage *syncMsg,
        BlockChangedEventTarget      eventTarget,
        Actor                       *blockChangeSource
    ) = 0;

    // vtb+5
    virtual void onBrightnessChanged(BlockSource &source, const BlockPos &pos) = 0;

    // vtb+6
    virtual void onBlockEntityChanged(BlockSource &source, BlockActor &te) = 0;

    // vtb+7
    virtual void onEntityChanged(BlockSource &source, Actor &entity) = 0;

    // vtb+8
    virtual void onBlockEvent(BlockSource &source, int x, int y, int z, int b0, int b1) = 0;
};