#pragma once

#include "SDK/api/src/common/world/level/BlockPos.h"
#include "SDK/api/src-deps/Core/Math/Color.h"

class Block;
class BlockSource;

namespace BlockTessellatorCustomExtraData {

    // size: 24
    class CauldronData {
    public:
        mce::Color mColor;        // off+0
        int        mTextureIndex; // off+16
        int        mRenderLayer;  // off+20
    };

    // size: 8
    class FlowerPotData {
    public:
        const Block *mPlant; // off+0
    };

    // size: 128
    class Map {
    public:
        std::unordered_map<BlockPos, CauldronData>  mExtraCauldronData;  // off+0
        std::unordered_map<BlockPos, FlowerPotData> mExtraFlowerPotData; // off+64
    };

    SAPPHIRE_API("1.21.2", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x4D\x8B\xF1\x4D\x8B\xF8\x48\x8B\xF2")
    SAPPHIRE_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\xB4\x24\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x4D\x8B\xF9\x4D\x8B\xE0\x48\x8B\xF2")
    SDK_API Map extract(BlockSource &region, const BlockPos &min, const BlockPos &max);

} // namespace BlockTessellatorCustomExtraData