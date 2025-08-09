#pragma once

#include "SDK/api/src-deps/Core/Threading/MPMCQueue.h"

class LevelChunk;

struct LevelChunkFinalDeleter {
    void operator()(LevelChunk *lc);
};

using TrackedUniqueChunkPtr = std::unique_ptr<LevelChunk, LevelChunkFinalDeleter>;

// size: 632
class LevelChunkGarbageCollector {
public:
    friend class Dimension;
    friend class LevelChunkFinalDeleter;

    Dimension                       &mDimension;            // off+0
    MPMCQueue<TrackedUniqueChunkPtr> mLevelChunksToDiscard; // off+8
    std::atomic<size_t>              mPendingDeletes = 0;   // off+624
};