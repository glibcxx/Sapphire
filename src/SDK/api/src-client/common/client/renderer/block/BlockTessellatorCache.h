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

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x89\x51\x00\x48\x8B\xD9")
    SDK_API void reset(BlockSource *region, const BlockPos &startPostion);

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x41\xBC\x00\x00\x00\x00\x33\xF6")
    SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x45\x33\x00\x4C\x8D\x5C\x24")
    SDK_API BrightnessPair getLightColor(const BlockPos &p, void *a2);
};