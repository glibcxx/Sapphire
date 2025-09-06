#pragma once

#include "SDK/api/src/common/world/level/LevelListener.h"
#include "SDK/api/src/common/world/level/SubChunkPos.h"
#include "SDK/api/src/common/world/level/dimension/DimensionType.h"

class RenderChunkShared;
class Level;
class LevelRenderer;
class LevelRendererCamera;
class RenderChunkCoordinatorProxy;
struct BlockActorBlockSyncMessage;

// size: 352 (1.21.50)
class RenderChunkCoordinator : public LevelListener {
public:
    // size: 8
    struct DirtyChunkData {
        bool           mImmediate;         // off+0
        bool           mChangesVisibility; // off+1
        std::bitset<6> mInterlockBitField; // off+4
    };

    float mSweepAndPruneRatio; // off+8

    using RenderChunkMapType = std::unordered_map<SubChunkPos, std::weak_ptr<RenderChunkShared>>;

    RenderChunkMapType::const_iterator mSweepAndPruneIterator;           // off+16
    RenderChunkMapType                 mRenderChunkSharedMap;            // off+24
    Level                             &mLevel;                           // off+88
    LevelRenderer                     &mLevelRenderer;                   // off+96
    std::vector<LevelRendererCamera *> mLevelRendererCameraListenerList; // off+104
    DimensionType                      mDimensionId;                     // off+128

    std::unordered_map<SubChunkPos, DirtyChunkData>                     mVisibilityDirtyRenderChunkMap;           // off+136
    std::unordered_map<SubChunkPos, std::vector<ActorBlockSyncMessage>> mRenderChunkChangedEntityNotificationMap; // off+200
    std::unordered_map<SubChunkPos, std::vector<BlockActorBlockSyncMessage>>
        mRenderChunkChangedBlockEntityNotificationMap; // off+264

    std::unique_ptr<RenderChunkCoordinatorProxy> mProxy;  // off+328
    std::shared_ptr<void>                        mUnk336; // off+336

    SDK_API RenderChunkCoordinator(Level &level, LevelRenderer &levelRenderer, AutomaticID<Dimension, int> dimensionID);
#pragma SPHR_LINKER_SYM_ALIAS(                                                                               \
    "??0RenderChunkCoordinator@@QEAA@AEAVLevel@@AEAVLevelRenderer@@V?$AutomaticID@VDimension@@H@@@Z",        \
    "?ctor@RenderChunkCoordinator@@QEAAPEAV1@AEAVLevel@@AEAVLevelRenderer@@V?$AutomaticID@VDimension@@H@@@Z" \
)

    SDK_API RenderChunkCoordinator *ctor(Level &level, LevelRenderer &levelRenderer, AutomaticID<Dimension, int> dimensionID);

    // vtb+0
    virtual ~RenderChunkCoordinator() override;

    SDK_API void tick();

    SDK_API std::shared_ptr<RenderChunkShared> getOrCreateChunkAtPos(const SubChunkPos &pos);

    SDK_API void _handleVisibilityUpdates();
};
