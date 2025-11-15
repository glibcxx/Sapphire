#pragma once

#include "SDK/api/src/common/world/level/BlockPos.h"

class BlockSource;

namespace BlockTessellatorCustomExtraData {

    class CauldronData;
    class FlowerPotData;

    // size: 128
    class Map {
    public:
        std::unordered_map<BlockPos, CauldronData>  mExtraCauldronData;  // off+0
        std::unordered_map<BlockPos, FlowerPotData> mExtraFlowerPotData; // off+64
    };

    SDK_API Map extract(BlockSource &region, const BlockPos &min, const BlockPos &max);

} // namespace BlockTessellatorCustomExtraData