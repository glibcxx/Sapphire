#pragma once

#include "SDK/api/src/common/CommonTypes.h"
#include "SDK/api/src/common/world/level/BlockPos.h"

class Block;
class BlockSource;
class BlockGraphics;

// size: 272032
class BlockTessellatorCache {
public:
    __int64                                 mUnk0;             // off+0
    BlockPos                                mPos;              // off+0
    BlockSource                            *mRegion;           // off+16
    std::array<BrightnessPair, 8000>        mLightColors;      // off+24
    std::array<const Block *, 8000>         mBlocks;           // off+16024
    std::array<const Block *, 8000>         mExtraBlocks;      // off+80024
    std::array<const BlockGraphics *, 8000> mBlockGraphics[2]; // off+144024
};