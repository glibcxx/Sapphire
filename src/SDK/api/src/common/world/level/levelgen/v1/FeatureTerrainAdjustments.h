#pragma once

#include <unordered_map>
#include <shared_mutex>
#include "SDK/api/src/common/world/level/ChunkPos.h"

class BeardDescription;
class BeardAndShaverDescription;
class BuryDescription;

// size: 72
class FeatureTerrainAdjustments {
public:
    using TerrainAdjustmentToken = std::shared_ptr<bool>;

    // size: 72
    struct Descriptions {
        std::vector<std::pair<BeardDescription, std::weak_ptr<bool>>>          mBeardifiers; // off+0
        std::vector<std::pair<BeardAndShaverDescription, std::weak_ptr<bool>>> mBeardAndShavers; // off+24
        std::vector<std::pair<BuryDescription, std::weak_ptr<bool>>>           mBurys; // off+48
    };

    std::unordered_map<ChunkPos, Descriptions> mDescriptionsToProcess; // off+0
    std::shared_mutex                          mMutex;                 // off+64
};