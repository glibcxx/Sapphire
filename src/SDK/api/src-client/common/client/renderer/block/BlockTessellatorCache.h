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
    BlockPos                                mPos;              // off+8
    BlockSource                            *mRegion;           // off+24
    std::array<BrightnessPair, 8000>        mLightColors;      // off+32
    std::array<const Block *, 8000>         mBlocks;           // off+16032
    std::array<const Block *, 8000>         mExtraBlocks;      // off+80032
    std::array<const BlockGraphics *, 8000> mBlockGraphics[2]; // off+144032

    SDK_API void reset(BlockSource *region, const BlockPos &startPostion);

    SDK_API BrightnessPair getLightColor(const BlockPos &p, void *a2);
};