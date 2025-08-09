#pragma once

#include "SDK/api/src/common/nbt/CompoundTag.h"

// size: 40
class LevelChunkMetaData {
    CompoundTag mMetaData;
    std::size_t mCurrentHash;
    bool        mHashNeedsRecomputing;
};