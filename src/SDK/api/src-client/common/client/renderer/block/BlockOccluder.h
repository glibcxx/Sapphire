#pragma once

#include "SDK/api/src/common/world/phys/AABB.h"

class Block;
class BlockTessellatorCache;
struct AirAndSimpleBlockBits;

// size: 16
class BlockOccluder {
public:
    std::bitset<6>         mFacingOccluded; // off+0
    BlockTessellatorCache &mBlockCache;     // off+8

    BlockOccluder(
        BlockTessellatorCache            &cache,
        const Block                      &block,
        const BlockPos                   &blockPos,
        const AABB                       &shape,
        const std::vector<unsigned char> &checkFace,
        bool                              renderingGUI,
        const AirAndSimpleBlockBits      *airAndSimpleBlocks
    );
#pragma SPHR_LINKER_SYM_ALIAS(                                                                                                                                             \
    "??0BlockOccluder@@QEAA@AEAVBlockTessellatorCache@@AEBVBlock@@AEBVBlockPos@@AEBVAABB@@AEBV?$vector@EV?$allocator@E@std@@@std@@_NPEBUAirAndSimpleBlockBits@@@Z",        \
    "?ctor@BlockOccluder@@QEAAPEAV1@AEAVBlockTessellatorCache@@AEBVBlock@@AEBVBlockPos@@AEBVAABB@@AEBV?$vector@EV?$allocator@E@std@@@std@@_NPEBUAirAndSimpleBlockBits@@@Z" \
)

    BlockOccluder *ctor(
        BlockTessellatorCache            &cache,
        const Block                      &block,
        const BlockPos                   &blockPos,
        const AABB                       &shape,
        const std::vector<unsigned char> &checkFace,
        bool                              renderingGUI,
        const AirAndSimpleBlockBits      *airAndSimpleBlocks
    );
};