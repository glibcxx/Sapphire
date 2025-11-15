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

    SDK_API Map extract(BlockSource &region, const BlockPos &min, const BlockPos &max);

} // namespace BlockTessellatorCustomExtraData