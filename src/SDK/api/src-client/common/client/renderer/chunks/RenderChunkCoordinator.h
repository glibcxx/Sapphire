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

    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\xEB\x00\x49\x8B\xF6\x0F\x57\xC0\x0F\x11\x44\x24")
    SPHR_CTOR_ALIAS SPHR_CTOR_ALIAS SDK_API RenderChunkCoordinator *ctor(
        Level &level, LevelRenderer &levelRenderer, AutomaticID<Dimension, int> dimensionID
    );

    // vtb+0
    virtual ~RenderChunkCoordinator() override;

    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x48\x8B\x03\x48\x8B\xD8\x48\x3B\xC6\x75\x00\x48\x8B\x8F")
    SDK_API void tick();

    SPHR_DECL_API("1.21.50", "\x40\x53\x55\x56\x57\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x4D\x8B\xF8")
    SDK_API std::shared_ptr<RenderChunkShared> getOrCreateChunkAtPos(const SubChunkPos &pos);

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x48\x8B\x03\x48\x8B\xD8\x48\x3B\xC6\x75\x00\x4D\x8B\xC6")
    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x48\x8B\x03\x48\x8B\xD8\x48\x3B\xC7\x75\x00\x4D\x8B\xC5")
    SPHR_DECL_API("1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x8B\x03\x48\x8B\xD8\x48\x3B\xC7\x75\x00\x4C\x8B\xC6")
    SDK_API void _handleVisibilityUpdates();
};
