#pragma once

#include "SDK/api/src/common/world/level/BlockPos.h"

namespace BlockTessellatorCustomExtraData {

    class CauldronData;
    class FlowerPotData;

    // size: 128
    class Map {
    public:
        std::unordered_map<BlockPos, CauldronData>  mExtraCauldronData;  // off+0
        std::unordered_map<BlockPos, FlowerPotData> mExtraFlowerPotData; // off+64
    };

} // namespace BlockTessellatorCustomExtraData