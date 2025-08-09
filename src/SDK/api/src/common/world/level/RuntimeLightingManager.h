#pragma once

#include "SDK/api/src/common/CommonTypes.h"
#include "ChunkPos.h"
#include "SubChunkBlockPos.h"
#include "SubChunkPos.h"

class Dimension;

struct SubChunkLightUpdate {
    SubChunkBlockPos mPos;           // off+0
    Brightness       mOldBrightness; // off+4
    Brightness       mNewBrightness; // off+5
    Brightness       mOldAbsorption; // off+6
    Brightness       mNewAbsorption; // off+7
    bool             mIsSkyLight;    // off+8
};

// size: 168
class RuntimeLightingManager {
public:
    // size: 32
    struct RelightingChunkElement {
        float                             mDist;             // off+0
        ChunkPos                          mChunkPos;         // off+8
        size_t                            mSubChunkIndex;    // off+16
        std::vector<SubChunkLightUpdate> *mAlteredBlockList; // off+24
    };

    // size: 24
    struct RuntimeLightingSubchunkList {
        std::vector<std::vector<SubChunkLightUpdate>> mAlteredSubchunkBlockList; // off+0
    };

    std::unordered_map<ChunkPos, RuntimeLightingSubchunkList> mLevelChunksToLight;    // off+0
    std::vector<RelightingChunkElement>                       mListOfChunksToProcess; // off+64
    std::vector<SubChunkPos>                                  mProcessedSubchunks;    // off+88
    std::vector<BlockPos>                                     mBrightnessChangedList; // off+112
    Dimension                                                &mDimension;             // off+136
    bool                                                      mWorkerScheduled;       // off+144
    std::chrono::steady_clock::duration                       mLightingTimeboxTime;   // off+152

    virtual ~RuntimeLightingManager() = default;
};