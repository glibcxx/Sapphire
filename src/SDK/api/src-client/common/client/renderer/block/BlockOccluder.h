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

    SDK_API BlockOccluder(
        BlockTessellatorCache            &cache,
        const Block                      &block,
        const BlockPos                   &blockPos,
        const AABB                       &shape,
        const std::vector<unsigned char> &checkFace,
        bool                              renderingGUI,
        const AirAndSimpleBlockBits      *airAndSimpleBlocks
    );

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x33\xC0\x4D\x8B\xF1")
    SPHR_CTOR_ALIAS SDK_API BlockOccluder *ctor(
        BlockTessellatorCache            &cache,
        const Block                      &block,
        const BlockPos                   &blockPos,
        const AABB                       &shape,
        const std::vector<unsigned char> &checkFace,
        bool                              renderingGUI,
        const AirAndSimpleBlockBits      *airAndSimpleBlocks
    );
};