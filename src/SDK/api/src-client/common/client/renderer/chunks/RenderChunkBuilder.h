#pragma once

#include "RenderChunkGeometry.h"
#include "SDK/api/src/common/world/phys/AABB.h"
#include <memory>

class Block;
class ChunkViewSource;
class BlockTessellator;
class Tessellator;
class RenderChunkCoordinator;
namespace ClientBlockPipeline {
    class MaterialRepository;
    class BlockTessellatorPipeline;
    class Description;
} // namespace ClientBlockPipeline
namespace mce {
    class MeshData;
}
namespace mce::framebuilder {
    struct FrameLightingModelCapabilities;
}
enum class BakedBlockLightType : int;

// size: 16
struct RenderChunkQuadInfo {
    uint32_t index   : 31; // off+0
    uint32_t reverse : 1;  // off+0
    Vec3     centroid;     // off+4
};

// size: 24
struct BlockQueueEntry {
    BlockPos     pos;       // off+0
    const Block &blockInfo; // off+16
};

// size: 568 (1.21.50)
class RenderChunkBuilder {
public:
    std::shared_ptr<ChunkViewSource>                mLocalSource;            // off+0
    std::unique_ptr<BlockTessellator>               mBlockTessellator;       // off+16
    bool                                            mAllDark;                // off+24
    bool                                            mSkyLit;                 // off+25
    bool                                            mBlendCanRenderAsOpaque; // off+26
    float                                           mAverageSkyLight;        // off+28
    std::array<std::vector<RenderChunkQuadInfo>, 7> mFaceMetadata;           // off+32
    std::array<RangeIndices, 15>                    mRenderLayerRanges;      // off+200
    std::vector<BlockQueueEntry>                   *mQueues;                 // off+320
    std::vector<BlockQueueEntry>                   *mSimpleOpaqueBlockQueue; // off+328
    uint64_t                                        mQueueIndexCounts[15];   // off+336
    std::unique_ptr<Tessellator>                    mOwnedTessellator;       // off+456

    std::shared_ptr<ClientBlockPipeline::MaterialRepository>       mMaterialRepository;  // off+464
    std::unique_ptr<ClientBlockPipeline::BlockTessellatorPipeline> mPipelineTessellator; // off+480
    std::unique_ptr<ClientBlockPipeline::Description>              mPipelineDescription; // off+488

    Tessellator                          &mTessellator;            // off+496
    std::unique_ptr<mce::Mesh>            mBuiltMesh;              // off+504
    std::unique_ptr<mce::MeshData>        mMeshData;               // off+512
    bool                                  mGUIRendering;           // off+520
    AABB                                  mBuildBoundingBox;       // off+524
    bool                                  mUnk548;                 // off+548
    std::weak_ptr<RenderChunkCoordinator> mRenderChunkCoordinator; // off+552

    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x8B\x47\x00\x48\x8D\x54\x24\x00\x89\x03")
    SDK_API void build(
        RenderChunkGeometry                                     &renderChunkGeometry,
        bool                                                     transparentLeaves,
        BakedBlockLightType                                      lightingType,
        bool                                                     forExport,
        const mce::framebuilder::FrameLightingModelCapabilities &lightingModelCapabilities
    );

    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x48\x8B\x86\x00\x00\x00\x00\x48\xC7\x80")
    SDK_API void _updateFacesMetadata();
};
