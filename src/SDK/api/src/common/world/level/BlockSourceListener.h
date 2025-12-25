#pragma once

#include "SDK/api/src/common/world/level/BlockPos.h"

class Actor;
class Block;
class BlockActor;
class BlockSource;
struct ActorBlockSyncMessage;

enum class BlockChangedEventTarget : uint8_t {
    SelfBlock = 0,
    NeighborBlock = 1,
};

// size: 8
class BlockSourceListener {
public:
    // vtb+0
    virtual ~BlockSourceListener() = default;

    // vtb+1
    virtual void onSourceCreated(BlockSource &source) {}

    // vtb+2
    virtual void onSourceDestroyed(BlockSource &source) {}

    // vtb+3
    virtual void onAreaChanged(BlockSource &source, const BlockPos &min, const BlockPos &max) {}

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
    ) {}

    // vtb+5
    virtual void onBrightnessChanged(BlockSource &source, const BlockPos &pos) {
        this->onAreaChanged(source, pos, pos);
    }

    // vtb+6
    virtual void onBlockEntityChanged(BlockSource &source, BlockActor &te) {}

    // vtb+7
    virtual void onEntityChanged(BlockSource &source, Actor &entity) {}

    // vtb+8
    virtual void onBlockEvent(BlockSource &source, int x, int y, int z, int b0, int b1) {}
};