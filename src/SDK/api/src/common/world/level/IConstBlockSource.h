#pragma once

#include "SDK/api/src/common/world/level/BlockPos.h"

class Block;
class BlockActor;

// size: 8
class IConstBlockSource {
public:
    // vtb+0
    virtual ~IConstBlockSource() = default;

    // vtb+1
    virtual const Block &getBlock(const BlockPos &pos, uint32_t layer) const = 0;

    // vtb+2
    virtual const Block &getBlock(const BlockPos &pos) const = 0;

    // vtb+3
    virtual const Block &getBlock(int x, int y, int z) const = 0;

    // vtb+4
    virtual const BlockActor *getBlockEntity(const BlockPos &pos) const = 0;

    // vtb+5
    virtual const Block &getExtraBlock(const BlockPos &pos) const = 0;

    // vtb+6
    virtual const Block &getLiquidBlock(const BlockPos &pos) const = 0;
};
