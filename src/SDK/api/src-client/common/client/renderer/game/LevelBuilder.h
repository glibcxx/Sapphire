#pragma once

#include "LevelRenderPreRenderUpdateParameters.h"
#include "PolygonOperatorPool.h"
#include "SDK/api/src-deps/Core/Threading/Async.h"
#include "SDK/api/src/common/world/level/SubChunkPos.h"
#include "SDK/api/src-deps/Core/Threading/MPMCQueue.h"
#include "SDK/api/src-deps/Platform/Threading/SpinLock.h"
#include <memory>

class RenderChunkInstanced;
class ChunkVisibilityCache;
class LevelCullerBase;
class RenderChunkBuilder;
class RenderChunkSorter;
class RenderChunkShared;
class LevelRenderer;
class LevelRendererCamera;
class RenderChunkSorterSharedInfo;
class SubChunkRequestSubscriber;
struct LevelRenderPreRenderUpdateParameters;
struct ChunkRenderObjectCollection;

// size: 1496
class LevelBuilder {
public:
    // size: 16
    struct PositionAndPriority {
        SubChunkPos mPosition; // off+0
        int         mPriority; // off+12
    };

    int                                                       maxCullingSteps;                             // off+0
    int                                                       numDirtyChunks;                              // off+4
    int                                                       numDirtyChunksPrevFrame;                     // off+8
    const int                                                 mMaxInflightChunks;                          // off+12
    bool                                                      mEnforceInterlocks;                          // off+16
    bool                                                      bCheckRightCuller;                           // off+17
    PolygonOperatorPool<RenderChunkBuilder>                   mBuilders;                                   // off+24
    PolygonOperatorPool<RenderChunkSorter>                    mSorters;                                    // off+160
    LevelRenderer                                            &mLevelRenderer;                              // off+296
    std::vector<std::shared_ptr<LevelRendererCamera>>         mLevelRendererCameras;                       // off+304
    MPMCQueue<SubChunkPos>                                    mRenderChunksToBuild;                        // off+328
    std::queue<std::shared_ptr<RenderChunkShared>>            mRenderChunksToUpload;                       // off+944
    SpinLock                                                  mRenderChunksFinishedBuildingSpinLock;       // off+984
    std::atomic<int>                                          mImmediateChunkInFlightCount;                // off+1016
    SpinLock                                                  mRenderChunksToQueryForCullerLock;           // off+1024
    std::vector<SubChunkPos>                                  mRenderChunksToQueryForCuller;               // off+1056
    std::vector<RenderChunkInstanced *>                       mSortedRenderChunkInstancedList;             // off+1080
    std::vector<PositionAndPriority>                          mSortedList;                                 // off+1104
    std::vector<SubChunkPos>                                  mRenderChunksVisibleFromCullingPoint;        // off+1128
    std::vector<SubChunkPos>                                  mRenderChunksVisibleFromCullingPoint2;       // off+1152
    std::unordered_set<SubChunkPos>                           mInterlockedRenderChunksToCheck;             // off+1176
    std::unordered_set<SubChunkPos>                           mInterlockGraphWalkSet;                      // off+1240
    std::vector<SubChunkPos>                                  mInterlockGraphWalkToCheckList;              // off+1304
    std::unordered_set<SubChunkPos>                           mExtraChunksToDrawUntilNextCull;             // off+1328
    std::vector<SubChunkPos>                                  mExtraChunksToDrawUntilNextCullEraseList;    // off+1392
    uint32_t                                                  mLastCullIteration;                          // off+1416
    size_t                                                    mLastNumRenderChunksVisibleFromCullingPoint; // off+1424
    std::chrono::steady_clock::time_point                     mLastTimeRenderChunksBeingBuilt;             // off+1432
    Bedrock::Threading::IAsyncResult<void>::Handle            mBuilderTask;                                // off+1440
    std::unique_ptr<SubChunkRequestSubscriber>                mSubChunkRequestSubscriber;                  // off+1456
    std::vector<std::shared_ptr<RenderChunkSorterSharedInfo>> mSharedSortInfo;                             // off+1464
    std::chrono::steady_clock::time_point                     mCurrentTime;                                // off+1488

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\xB4\x24\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x45\x0F\xB6\xE0")
    SPHR_DECL_API("1.21.50", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\xB4\x24\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x4C\x8B\xF2\x48\x89\x55\x00\x4C\x8B\xF9")
    SPHR_DECL_API("1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\xB4\x24\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x4C\x8B\xFA\x48\x89\x55")
    SDK_API void startLevelBuildForThisFrame(const LevelRenderPreRenderUpdateParameters &levelRenderPreRenderUpdateParameters);

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x49\x8B\xBE\x00\x00\x00\x00\x4D\x8B\xBE\x00\x00\x00\x00\x49\x3B\xFF\x0F\x84")
    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x49\x8B\xB7\x00\x00\x00\x00\x49\x8B\x87")
    SPHR_DECL_API("1.21.60", "call", "\xE8\x00\x00\x00\x00\x49\x8B\xB6\x00\x00\x00\x00\x4D\x8B\xAE")
    SDK_API void _buildRenderChunks(const Vec3 &cameraPos, const Vec3 &viewAreaCenter);

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x4D\x8B\xE9\x48\x8B\xF2")
    SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x4C\x8B\x44\x24\x00\xE9")
    SDK_API void _prepareRenderChunkRenderListProcessRenderChunkInstanced(
        RenderChunkInstanced &renderChunkInstanced,
        const Vec3           &cameraCenter,
        int32_t              &brightnessCountChunks,
        float                &totalBrightness,
        bool                  a6
    );

    SPHR_DECL_API("1.21.2", "disp:15,call", "\x0F\x28\xDE\x4C\x8D\x44\x24\x00\x48\x8D\x54\x24\x00\x49\x8B\xCE")
    SPHR_DECL_API("1.21.50", "disp:15,call", "\x0F\x28\xDE\x4C\x8D\x44\x24\x00\x48\x8D\x55\x00\x49\x8B\xCF")
    SPHR_DECL_API("1.21.60", "disp:15,call", "\x0F\x28\xDE\x4C\x8D\x45\x00\x48\x8D\x55\x00\x49\x8B\xCE")
    SDK_API void _prepareRenderChunkRenderList(
        const Vec3 &viewAreaCenter, const Vec3 &cameraCenter, float currentTime, bool underWater
    );

    SPHR_DECL_API("1.21.50", "disp:3,call", "\x49\x8B\xCE\xE8\x00\x00\x00\x00\x41\x80\x7E\x00\x00\x74")
    SDK_API void _walkInterlockedGraph();

    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\xEB\x00\xB0\x00\x0F\xB6\xD0")
    SDK_API bool _checkGraphFreeToUnlock() const;

    SPHR_DECL_API("1.21.50", "disp:6,call", "\x0F\xB6\xD0\x49\x8B\xCE\xE8\x00\x00\x00\x00\x49\x83\xBE")
    SDK_API void _unlockOrBuildSortGraph(bool freeToUnlock);

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\xF3\x0F\x10\x76\x00\x49\x8B\x86")
    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\xF3\x41\x0F\x10\x76\x00\x49\x8B\x87")
    SPHR_DECL_API("1.21.60", "call", "\xE8\x00\x00\x00\x00\xF3\x41\x0F\x10\x77\x00\x49\x8B\x86")
    SDK_API void _handleCullerQueries();
};
