#pragma once

class RenderChunkShared;
class RenderChunkGeometry;
struct RangeIndices;
struct RenderChunkDirectIndexData;
namespace mce {
    class IndexBufferContainer;
    struct BufferResourceService;
} // namespace mce
namespace mce::framebuilder {
    struct FrameLightingModelCapabilities;
}
enum class BakedBlockLightType : int;

// size: 200 (1.21.50)
class RenderChunkInstanced {
public:
    enum class SortState : uint8_t {
        Sorting = 0,
        Idle = 1,
    }; // SortState

    float mDistanceFromCamera2; // off+0
    float mChunkBuildPriority;  // off+4

    bool mCanRender                      : 1; // off+8
    bool mCameraMovedEnough              : 1; // off+8
    bool mFallbackToUnsorted             : 1; // off+8
    bool mNewSortData                    : 1; // off+8
    bool mCurrentGeometryHasSortedLayers : 1; // off+8
    bool mEmpty                          : 1; // off+8

    std::atomic<SortState> mSortState;                          // off+9
    uint8_t                mLastSortRenderChunkGeometryVersion; // off+10
    float                  mAverageBrightness;                  // off+12

    std::shared_ptr<RenderChunkShared>   mRenderChunkShared;          // off+16
    std::shared_ptr<RenderChunkGeometry> mCurrentRenderChunkGeometry; // off+32
    std::variant<
        std::monostate,
        std::shared_ptr<mce::IndexBufferContainer>,
        std::shared_ptr<RenderChunkDirectIndexData>>
                                                  mCurrentIndices;             // off+48
    std::unique_ptr<std::array<RangeIndices, 18>> mCurrentIndexRanges;         // off+72
    std::shared_ptr<RenderChunkGeometry>          mSortingRenderChunkGeometry; // off+80
    std::shared_ptr<RenderChunkGeometry>          mNextRenderChunkGeometry;    // off+96
    uint64_t                                      mUnk104;                     // off+104
    std::variant<
        std::monostate,
        std::shared_ptr<mce::IndexBufferContainer>,
        std::shared_ptr<RenderChunkDirectIndexData>>
                                                  mNextIndices;                   // off+112
    std::unique_ptr<std::array<RangeIndices, 18>> mNextIndexRanges;               // off+144
    glm::tvec3<int>                               mLastSortCameraDelta;           // off+152
    glm::tvec3<int>                               mCurrentIndexBufferCameraDelta; // off+164
    std::chrono::steady_clock::time_point         mGuaranteedPromoteTime;         // off+176
    std::weak_ptr<mce::BufferResourceService>     mBufferResourceService;         // off+184

    SDK_API RenderChunkInstanced();
#pragma SDK_LINKER_SYM_ALIAS(                 \
    "??0RenderChunkInstanced@@QEAA@XZ",        \
    "?ctor@RenderChunkInstanced@@QEAAPEAV1@XZ" \
)

    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\xEB\x00\x49\x8B\xDF\x48\x8D\x43\x00\x48\x89\x45")
    SDK_API RenderChunkInstanced *ctor();
};