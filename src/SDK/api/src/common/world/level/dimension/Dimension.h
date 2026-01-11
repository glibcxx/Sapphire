#pragma once

#include <set>
#include "IDimension.h"
#include "SDK/api/src/common/CommonTypes.h"
#include "SDK/api/src/common/world/level/levelgen/v1/FeatureTerrainAdjustments.h"
#include "SDK/api/src/common/world/level/levelgen/v2/providers/IntProvider.h"
#include "SDK/api/src/common/world/level/chunk/LevelChunkGarbageCollector.h"
#include "SDK/api/src/common/world/level/saveddata/SavedData.h"
#include "SDK/api/src/common/world/level/LevelListener.h"
#include "SDK/api/src/common/world/level/SubChunkPos.h"
#include "SDK/api/src/common/gamerefs/EnableGetWeakRef.h"
#include "SDK/api/src/common/gamerefs/OwnerPtr.h"
#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"

class ILevel;
class BaseLightTextureImageBuilder;
class DimensionBrightnessRamp;
class LevelChunkMetaData;
class RuntimeLightingManager;
class BlockEventDispatcher;
class TaskGroup;
class PostprocessingManager;
class SubChunkInterlocker;
class WorldGenerator;
class Weather;
class Seasons;
class GameEventDispatcher;
class CircuitSystem;
class WeakEntityRef;
class WireframeQueue;
class TickingAreaList;
class LevelChunkBuilderData;
class ChunkBuildOrderPolicyBase;
class VillageManager;
class NetworkIdentifierWithSubId;
class ChunkLoadActionList;
class DelayActionList;
class UpdateSubChunkBlocksChangedInfo;
class Scheduler;

// https://github.com/LiteLDev/LeviLamina/blob/4d08de07ef76fb4f101aebb8b251e2652dc4cfbd/src/mc/world/level/dimension/Dimension.h#L79

// size: 16
struct ActorChunkTransferEntry {
    ChunkPos mOldChunkPos;
    ChunkPos mNewChunkPos;
};

// size: 12
class ChunkKey {
public:
    const ChunkPos      pos;
    const DimensionType id;
};

// size: 56
struct ActorUnloadedChunkTransferEntry {
    ChunkKey            mFromChunkKey;
    std::string         mActorStorageKey;
    Unique<CompoundTag> mActorTag;
};

// size: 4
class DimensionHeightRange {
public:
    Height mMin;
    Height mMax;
};

#if MC_VERSION >= v1_21_50

// size: 112
class ActorReplication {
    alignas(8) uint8_t _fill[112]; // off+0
};

#endif

#if MC_VERSION == v1_21_2

class PlayerReplicationStructures;

// size: 1576
class Dimension : public IDimension,
                  public LevelListener,
                  public SavedData,
                  public Bedrock::EnableNonOwnerReferences,
                  public EnableGetWeakRef<Dimension>,
                  public std::enable_shared_from_this<Dimension> {
public:
    std::vector<ActorChunkTransferEntry> mActorChunkTransferQueue; // off+104
    std::unordered_map<ChunkKey, std::vector<ActorUnloadedChunkTransferEntry>>
        mActorUnloadedChunkTransferQueue; // off+128

    ILevel              &mLevel;                       // off+192
    DimensionHeightRange mHeightRange;                 // off+200
    Height               mSeaLevel;                    // off+204
    uint8_t              mMonsterSpawnBlockLightLimit; // off+206

    OwnerPtr<BlockSource> mBlockSource;                // off+208
    float                 mMobsPerChunkSurface[7];     // off+224
    float                 mMobsPerChunkUnderground[7]; // off+252
    BrightnessPair        mDefaultBrightness;          // off+280

    std::unique_ptr<BaseLightTextureImageBuilder> mLightTextureImageBuilder; // off+288
    std::unique_ptr<DimensionBrightnessRamp>      mDimensionBrightnessRamp;  // off+396
    std::shared_ptr<LevelChunkMetaData>           mTargetMetaData;           // off+304
    std::unique_ptr<RuntimeLightingManager>       mRuntimeLightingManager;   // off+320

    std::string   mName; // off+328
    DimensionType mId;   // off+360

    bool       mUltraWarm;   // off+364
    bool       mHasCeiling;  // off+365
    bool       mHasWeather;  // off+366
    bool       mHasSkylight; // off+367
    Brightness mSkyDarken;   // off+368

    std::unique_ptr<BlockEventDispatcher>            mDispatcher;                // off+376
    std::unique_ptr<TaskGroup>                       mTaskGroup;                 // off+384
    std::unique_ptr<TaskGroup>                       mChunkGenTaskGroup;         // off+392
    std::unique_ptr<PostprocessingManager>           mPostProcessingManager;     // off+400
    std::unique_ptr<SubChunkInterlocker>             mSubChunkInterlocker;       // off+408
    std::unique_ptr<ChunkSource>                     mChunkSource;               // off+416
    WorldGenerator                                  *mWorldGenerator;            // off+424
    std::unique_ptr<Weather>                         mWeather;                   // off+432
    std::unique_ptr<Seasons>                         mSeasons;                   // off+440
    std::unique_ptr<GameEventDispatcher>             mGameEventDispatcher;       // off+448
    std::unique_ptr<CircuitSystem>                   mCircuitSystem;             // off+456
    const int                                        CIRCUIT_TICK_RATE;          // off+464
    int                                              mCircuitSystemTickRate;     // off+468
    std::unordered_map<ActorUniqueID, WeakEntityRef> mActorIDEntityIDMap;        // off+472
    std::vector<WeakEntityRef>                       mDisplayEntities;           // off+536
    std::shared_ptr<WireframeQueue>                  mWireframeQueue;            // off+560
    FeatureTerrainAdjustments                        mFeatureTerrainAdjustments; // off+576

    std::unordered_map<ChunkPos, std::vector<std::unique_ptr<CompoundTag>>> mLimboEntities; // off+648

    std::set<ActorUniqueID>          mEntitiesToMoveChunks;       // off+712
    std::shared_ptr<TickingAreaList> mTickingAreaList;            // off+728
    LevelChunkGarbageCollector       mLevelChunkGarbageCollector; // off+744
    std::set<ActorUniqueID>          mWitherIDs;                  // off+1376

    std::unique_ptr<LevelChunkBuilderData>                           mLevelChunkBuilderData;      // off+1392
    std::chrono::steady_clock::time_point                            mLastPruneTime;              // off+1400
    std::chrono::steady_clock::time_point                            mLastStructurePruneTime;     // off+1408
    std::unique_ptr<ChunkBuildOrderPolicyBase>                       mChunkBuildOrderPolicy;      // off+1416
    std::unique_ptr<VillageManager>                                  mVillageManager;             // off+1424
    std::vector<NetworkIdentifierWithSubId>                          mTemporaryPlayerIds;         // off+1432
    std::unique_ptr<ChunkLoadActionList>                             mChunkLoadActionList;        // off+1456
    std::unique_ptr<DelayActionList>                                 mDelayActionList;            // off+1464
    std::unordered_map<SubChunkPos, UpdateSubChunkBlocksChangedInfo> mBlocksChangedBySubChunkMap; // off+1472
    std::unique_ptr<PlayerReplicationStructures>                     mReplicationStructures;      // off+1536
    std::vector<WeakEntityRef>                                       mPlayersToReplicate;         // off+1544
    bool                                                             mRunChunkGenWatchDog;        // off+1568

    SDK_API Dimension(
        ILevel              &level,
        DimensionType        dimId,
        DimensionHeightRange heightRange,
        Scheduler           &callbackContext,
        std::string          name
    );

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x06\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x46\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x46\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x46\x00\xC6\x86")
    SPHR_CTOR_ALIAS SDK_API Dimension *ctor(
        ILevel              &level,
        DimensionType        dimId,
        DimensionHeightRange heightRange,
        Scheduler           &callbackContext,
        std::string          name
    );
};
static_assert(sizeof(Dimension) == 1576);

#elif MC_VERSION == v1_21_50

// size: 1712
class Dimension : public IDimension,
                  public LevelListener,
                  public SavedData,
                  public Bedrock::EnableNonOwnerReferences,
                  public EnableGetWeakRef<Dimension>,
                  public std::enable_shared_from_this<Dimension> {
public:
    std::vector<ActorChunkTransferEntry> mActorChunkTransferQueue; // off+104
    std::unordered_map<ChunkKey, std::vector<ActorUnloadedChunkTransferEntry>>
        mActorUnloadedChunkTransferQueue; // off+128

    ILevel              &mLevel;                       // off+192
    DimensionHeightRange mHeightRange;                 // off+200
    Height               mSeaLevel;                    // off+204
    uint8_t              mMonsterSpawnBlockLightLimit; // off+206

    IntProvider mMonsterSpawnLightTest; // off+208

    OwnerPtr<BlockSource> mBlockSource;                // off+240
    float                 mMobsPerChunkSurface[7];     // off+256
    float                 mMobsPerChunkUnderground[7]; // off+284
    BrightnessPair        mDefaultBrightness;          // off+312

    std::unique_ptr<BaseLightTextureImageBuilder> mLightTextureImageBuilder; // off+320
    std::unique_ptr<DimensionBrightnessRamp>      mDimensionBrightnessRamp;  // off+328
    std::shared_ptr<LevelChunkMetaData>           mTargetMetaData;           // off+336
    std::unique_ptr<RuntimeLightingManager>       mRuntimeLightingManager;   // off+352

    std::string   mName; // off+360
    DimensionType mId;   // off+392

    bool       mUltraWarm;   // off+396
    bool       mHasCeiling;  // off+397
    bool       mHasWeather;  // off+398
    bool       mHasSkylight; // off+399
    Brightness mSkyDarken;   // off+400

    std::unique_ptr<BlockEventDispatcher>            mDispatcher;                // off+408
    std::unique_ptr<TaskGroup>                       mTaskGroup;                 // off+416
    std::unique_ptr<TaskGroup>                       mChunkGenTaskGroup;         // off+424
    std::unique_ptr<PostprocessingManager>           mPostProcessingManager;     // off+432
    std::unique_ptr<SubChunkInterlocker>             mSubChunkInterlocker;       // off+440
    std::unique_ptr<ChunkSource>                     mChunkSource;               // off+448
    WorldGenerator                                  *mWorldGenerator;            // off+456
    std::unique_ptr<Weather>                         mWeather;                   // off+464
    std::unique_ptr<Seasons>                         mSeasons;                   // off+472
    std::unique_ptr<GameEventDispatcher>             mGameEventDispatcher;       // off+480
    std::unique_ptr<CircuitSystem>                   mCircuitSystem;             // off+488
    const int                                        CIRCUIT_TICK_RATE;          // off+496
    int                                              mCircuitSystemTickRate;     // off+500
    std::unordered_map<ActorUniqueID, WeakEntityRef> mActorIDEntityIDMap;        // off+504
    std::vector<WeakEntityRef>                       mDisplayEntities;           // off+568
    std::shared_ptr<WireframeQueue>                  mWireframeQueue;            // off+592
    FeatureTerrainAdjustments                        mFeatureTerrainAdjustments; // off+608

    std::unordered_map<ChunkPos, std::vector<std::unique_ptr<CompoundTag>>> mLimboEntities; // off+680

    std::set<ActorUniqueID>                                          mEntitiesToMoveChunks;       // off+744
    std::shared_ptr<TickingAreaList>                                 mTickingAreaList;            // off+760
    LevelChunkGarbageCollector                                       mLevelChunkGarbageCollector; // off+776
    std::set<ActorUniqueID>                                          mWitherIDs;                  // off+1408
    std::unique_ptr<LevelChunkBuilderData>                           mLevelChunkBuilderData;      // off+1424
    std::chrono::steady_clock::time_point                            mLastPruneTime;              // off+1432
    std::chrono::steady_clock::time_point                            mLastStructurePruneTime;     // off+1440
    std::unique_ptr<ChunkBuildOrderPolicyBase>                       mChunkBuildOrderPolicy;      // off+1448
    std::unique_ptr<VillageManager>                                  mVillageManager;             // off+1456
    std::vector<NetworkIdentifierWithSubId>                          mTemporaryPlayerIds;         // off+1464
    std::unique_ptr<ChunkLoadActionList>                             mChunkLoadActionList;        // off+1488
    std::unique_ptr<DelayActionList>                                 mDelayActionList;            // off+1496
    std::unordered_map<SubChunkPos, UpdateSubChunkBlocksChangedInfo> mBlocksChangedBySubChunkMap; // off+1504
    ActorReplication                                                 mActorReplication;           // off+1568
    std::vector<WeakEntityRef>                                       mPlayersToReplicate;         // off+1680
    bool                                                             mRunChunkGenWatchDog;        // off+1704

    SDK_API Dimension(
        ILevel              &level,
        DimensionType        dimId,
        DimensionHeightRange heightRange,
        Scheduler           &callbackContext,
        std::string          name
    );

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x06\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x46\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x46\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x46\x00\xC6\x86")
    SPHR_CTOR_ALIAS SDK_API Dimension *ctor(
        ILevel              &level,
        DimensionType        dimId,
        DimensionHeightRange heightRange,
        Scheduler           &callbackContext,
        std::string          name
    );
};
static_assert(sizeof(Dimension) == 1712);

#elif MC_VERSION == v1_21_60

// size: 1704
class Dimension : public IDimension,
                  public LevelListener,
                  public SavedData,
                  public Bedrock::EnableNonOwnerReferences,
                  public EnableGetWeakRef<Dimension>,
                  public std::enable_shared_from_this<Dimension> {
public:
    std::vector<ActorChunkTransferEntry> mActorChunkTransferQueue; // off+104
    std::unordered_map<ChunkKey, std::vector<ActorUnloadedChunkTransferEntry>>
        mActorUnloadedChunkTransferQueue; // off+128

    ILevel               &mLevel;                       // off+192
    DimensionHeightRange  mHeightRange;                 // off+200
    Height                mSeaLevel;                    // off+204
    uint8_t               mMonsterSpawnBlockLightLimit; // off+206
    IntProvider           mMonsterSpawnLightTest;       // off+208
    OwnerPtr<BlockSource> mBlockSource;                 // off+240
    bool                  mHasWeather;                  // off+256
    float                 mMobsPerChunkSurface[7];      // off+260
    float                 mMobsPerChunkUnderground[7];  // off+288
    BrightnessPair        mDefaultBrightness;           // off+316

    std::unique_ptr<BaseLightTextureImageBuilder> mLightTextureImageBuilder; // off+320
    std::unique_ptr<DimensionBrightnessRamp>      mDimensionBrightnessRamp;  // off+328
    std::shared_ptr<LevelChunkMetaData>           mTargetMetaData;           // off+336
    std::unique_ptr<RuntimeLightingManager>       mRuntimeLightingManager;   // off+352

    std::string   mName; // off+360
    DimensionType mId;   // off+392

    bool       mUltraWarm;   // off+396
    bool       mHasCeiling;  // off+397
    bool       mHasSkylight; // off+398
    Brightness mSkyDarken;   // off+399

    std::unique_ptr<BlockEventDispatcher>            mDispatcher;                // off+400
    std::unique_ptr<TaskGroup>                       mTaskGroup;                 // off+408
    std::unique_ptr<TaskGroup>                       mChunkGenTaskGroup;         // off+416
    std::unique_ptr<PostprocessingManager>           mPostProcessingManager;     // off+424
    std::unique_ptr<SubChunkInterlocker>             mSubChunkInterlocker;       // off+432
    std::unique_ptr<ChunkSource>                     mChunkSource;               // off+440
    WorldGenerator                                  *mWorldGenerator;            // off+448
    std::unique_ptr<Weather>                         mWeather;                   // off+456
    std::unique_ptr<Seasons>                         mSeasons;                   // off+464
    std::unique_ptr<GameEventDispatcher>             mGameEventDispatcher;       // off+472
    std::unique_ptr<CircuitSystem>                   mCircuitSystem;             // off+480
    const int                                        CIRCUIT_TICK_RATE;          // off+488
    int                                              mCircuitSystemTickRate;     // off+492
    std::unordered_map<ActorUniqueID, WeakEntityRef> mActorIDEntityIDMap;        // off+496
    std::vector<WeakEntityRef>                       mDisplayEntities;           // off+560
    std::shared_ptr<WireframeQueue>                  mWireframeQueue;            // off+584
    FeatureTerrainAdjustments                        mFeatureTerrainAdjustments; // off+600

    std::unordered_map<ChunkPos, std::vector<std::unique_ptr<CompoundTag>>> mLimboEntities; // off+672

    std::set<ActorUniqueID>                                          mEntitiesToMoveChunks;       // off+736
    std::shared_ptr<TickingAreaList>                                 mTickingAreaList;            // off+752
    LevelChunkGarbageCollector                                       mLevelChunkGarbageCollector; // off+768
    std::set<ActorUniqueID>                                          mWitherIDs;                  // off+1400
    std::unique_ptr<LevelChunkBuilderData>                           mLevelChunkBuilderData;      // off+1416
    std::chrono::steady_clock::time_point                            mLastPruneTime;              // off+1424
    std::chrono::steady_clock::time_point                            mLastStructurePruneTime;     // off+1432
    std::unique_ptr<ChunkBuildOrderPolicyBase>                       mChunkBuildOrderPolicy;      // off+1440
    std::unique_ptr<VillageManager>                                  mVillageManager;             // off+1448
    std::vector<NetworkIdentifierWithSubId>                          mTemporaryPlayerIds;         // off+1456
    std::unique_ptr<ChunkLoadActionList>                             mChunkLoadActionList;        // off+1480
    std::unique_ptr<DelayActionList>                                 mDelayActionList;            // off+1488
    std::unordered_map<SubChunkPos, UpdateSubChunkBlocksChangedInfo> mBlocksChangedBySubChunkMap; // off+1496
    ActorReplication                                                 mActorReplication;           // off+1560
    std::vector<WeakEntityRef>                                       mPlayersToReplicate;         // off+1672
    bool                                                             mRunChunkGenWatchDog;        // off+1696

    SDK_API Dimension(
        ILevel              &level,
        DimensionType        dimId,
        DimensionHeightRange heightRange,
        Scheduler           &callbackContext,
        std::string          name
    );

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x06\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x46\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x46\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x46\x00\xC6\x86")
    SPHR_CTOR_ALIAS SDK_API Dimension *ctor(
        ILevel              &level,
        DimensionType        dimId,
        DimensionHeightRange heightRange,
        Scheduler           &callbackContext,
        std::string          name
    );
};
static_assert(sizeof(Dimension) == 1704);

#endif