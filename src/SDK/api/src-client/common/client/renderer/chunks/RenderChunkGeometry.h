#pragma once

#include "SDK/api/src/common/world/level/Tick.h"
#include "SDK/api/src/common/util/Bounds.h"
#include "SDK/api/src/common/world/phys/Vec3.h"
#include "SDK/api/src-deps/Renderer/PointLight.h"

class RenderChunkQuadInfo;
class RenderChunkBuilder;
struct RenderChunkDirectVertexData;
namespace mce {
    class Mesh;
    struct BufferResourceService;
} // namespace mce
namespace dragon {
    struct RenderMetadata;
}

// size: 8
struct RangeIndices {
    int mStart; // off+0
    int mCount; // off+4
};

// size: 560 (1.21.50)
class RenderChunkGeometry {
public:
    float mAverageSkyLight;        // off+0
    bool  mImmediateChange;        // off+4
    bool  mBlendCanRenderAsOpaque; // off+5
    Tick  mLastChangeTick;         // off+8
    Tick  mFirstChangeTick;        // off+16
    std::variant<std::monostate, std::shared_ptr<mce::Mesh>, std::shared_ptr<RenderChunkDirectVertexData>>
        mMeshData; // off+24

    bool  mCanRender;                    // off+48
    bool  mBuilt;                        // off+49
    bool  mRayTracingModeOnAtBuildStart; // off+50
    bool  mRayTracingModeOnAtBuildEnd;   // off+51
    float mReadyTime;                    // off+52
    int   mBuildIterationCount;          // off+56

    BlockPos                             mPosition;                        // off+60
    BlockPos                             mCenter;                          // off+72
    Bounds                               mDataBounds;                      // off+84
    std::array<RangeIndices, 18>         mUnsortedIndexRange;              // off+132
    bool                                 mHasSortedLayers;                 // off+276
    std::array<std::shared_ptr<void>, 7> mFaceMetadata;                    // off+280
    std::array<RangeIndices, 15>         mRenderLayerIndexRanges;          // off+392
    uint8_t                              mBuildVersionNumber;              // off+512
    std::bitset<6>                       mInterlockBitField;               // off+516
    std::array<uint8_t, 6>               mInterlockNeighborVersionNumbers; // off+520
    std::vector<mce::PointLight>         mPointLights;                     // off+528
    uint32_t                             mUnk552;                          // off+552

    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\xB9\x00\x00\x00\x00\x33\xC0\xF0\x0F\xB1\x4B")
    SDK_API void endRebuild(
        RenderChunkBuilder           &builder,
        mce::BufferResourceService   &bufferResourceService,
        bool                          isBuilding,
        const dragon::RenderMetadata &renderMetadata,
        bool                          enforceInterlocks
    );
};