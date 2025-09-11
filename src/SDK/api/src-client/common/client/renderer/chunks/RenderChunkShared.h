#pragma once

#include "SDK/api/src/common/world/phys/Vec3.h"
#include "SDK/api/src/common/world/level/ActorBlockSyncMessage.h"
#include "SDK/api/src/common/world/level/BlockActorBlockSyncMessage.h"
#include "SDK/api/src-client/common/client/renderer/game/ChunkVisibilityCache.h"
#include "SDK/api/src-deps/MinecraftRenderer/framebuilder/RenderMetadata.h"
#include "SDK/api/src-deps/Core/Utility/buffer_span.h"

class RenderChunkGeometry;
class RenderChunkBuilder;
namespace mce::framebuilder {
    struct FrameLightingModelCapabilities;
}

enum class BakedBlockLightType : int {
    None = 0,
    Flat = 1,
    Smooth = 2,
};

// size: 200 (1.21.50)
class RenderChunkShared {
public:
    enum class VisibilityBuildState : int {
        Dirty = 0,
        Building = 1,
        BuildingButDirty = 2,
        Ready = 3,
    };

    enum class DataState : int {
        WontLoad = 0,
        Loading = 1,
        Loaded = 2,
    };

    enum class BuildState : int {
        Building = 0,
        NotBuldingOrSorting = 1,
    };

    bool                                 mEmpty;                       // off+0
    std::atomic<BuildState>              mBuildState;                  // off+4
    BlockPos                             mCenter;                      // off+8
    BlockPos                             mPosition;                    // off+20
    std::atomic<VisibilityBuildState>    mVisibilityBuildState;        // off+32
    std::shared_ptr<RenderChunkGeometry> mCurrentRenderChunkGeometry;  // off+40
    VisibilityNode                       mVisibility;                  // off+56
    bool                                 mVisibilityMatrixChanged;     // off+62
    bool                                 mSkyLit;                      // off+63
    bool                                 mAllDark;                     // off+64
    bool                                 mUnk65;                       // off+65
    bool                                 mImmediateChangeRequested;    // off+66
    std::shared_ptr<RenderChunkGeometry> mBuildingRenderChunkGeometry; // off+72

    // size: 24
    struct ActorBlockSyncMessageWithVersion {
        ActorBlockSyncMessage mEntityBlockSyncMessage;     // off+0
        uint8_t               mRenderChunkGeometryVersion; // off+16
    };

    // size: 20
    struct BlockActorBlockSyncMessageWithVersion {
        BlockActorBlockSyncMessage mBlockEntityBlockSyncMessage; // off+0
        uint8_t                    mRenderChunkGeometryVersion;  // off+16
    };

    std::vector<ActorBlockSyncMessageWithVersion>      mEntityBlockMessageList;      // off+88
    std::vector<BlockActorBlockSyncMessageWithVersion> mBlockEntityBlockMessageList; // off+112
    dragon::RenderMetadata                             mRenderMetadata;              // off+136

    SDK_API RenderChunkShared(const BlockPos &p);
#pragma SPHR_LINKER_SYM_ALIAS("??0RenderChunkShared@@QEAA@AEBVBlockPos@@@Z", "?ctor@RenderChunkShared@@QEAAPEAV1@AEBVBlockPos@@@Z")

    SDK_API RenderChunkShared *ctor(const BlockPos &p);

    SDK_API void startRebuild(RenderChunkBuilder &builder, const Vec3 &currentCameraPosition);

    SDK_API void rebuild(
        RenderChunkBuilder                                      &builder,
        bool                                                     transparentLeaves,
        BakedBlockLightType                                      lightingType,
        bool                                                     forExport,
        const mce::framebuilder::FrameLightingModelCapabilities &lightingModelCapabilities
    );

    SDK_API void addEntityBlockSyncMessages(buffer_span<ActorBlockSyncMessage> syncMsgList);

    SDK_API void addBlockEntityBlockSyncMessages(buffer_span<BlockActorBlockSyncMessage> syncMsgList);
};