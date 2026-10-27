#pragma once

#include "SDK/api/src-deps/MinecraftRenderer/game/RangeIndices.h"
#include "RenderChunkSorterCameraInfo.h"
#include "SDK/api/src/common/world/level/SubChunkPos.h"
#include "SDK/api/src-client/common/client/renderer/TessellatorQuadInfo.h"
#include "SDK/api/src-deps/Core/Utility/buffer_span.h"

// size: 336 (1.21.50)
class RenderChunkSorter {
public:
    // size: 12
    struct FaceInfo {
    public:
        float    distance; // off+0
        uint32_t index;    // off+4
        uint32_t reverse;  // off+8
    };

    std::vector<uint8_t>                         mRawIndexBytes;       // off+8
    std::array<RangeIndices, 15>                 mSortedLayers;        // off+32
    std::array<RangeIndices, 15>                 mLayers;              // off+152
    std::vector<RenderChunkSorter::FaceInfo>     mFaceInfo;            // off+272
    uint32_t                                     mIndexSize;           // off+296
    glm::tvec3<int>                              mCameraDelta;         // off+300
    std::shared_ptr<RenderChunkSorterSharedInfo> mSharedInfo;          // off+312
    bool                                         mForceDisableCulling; // off+328

    // vtb+0
    virtual ~RenderChunkSorter() noexcept;

    SPHR_DECL_API("1.21.50", "disp:5,call", "\x48\x8D\x54\x24\x00\xE8\x00\x00\x00\x00\x90\x48\x8B\xC7\x48\x8B\x5C\x24\x00\x48\x8B\x74\x24")
    SDK_API RenderChunkSorter(std::shared_ptr<RenderChunkSorterSharedInfo> sharedInfo);

    SPHR_CTOR_ALIAS SDK_API RenderChunkSorter *ctor(std::shared_ptr<RenderChunkSorterSharedInfo> sharedInfo);

    // vtb+1
    virtual void sortAndCullFaces(const SubChunkPos &renderChunkPos, buffer_span<std::vector<RenderChunkQuadInfo>> faces);
};

// size: 392 (1.21.50)
class DeferredRenderingRenderChunkSorter : public RenderChunkSorter {
    __int64                                      mUnk336[3]; // off+
    std::shared_ptr<RenderChunkSorterSharedInfo> mUnk360;    // off+360
    std::shared_ptr<RenderChunkSorterSharedInfo> mUnk376;    // off+376

    SPHR_DECL_API("1.21.50", "disp:13,call", "\x4C\x8D\x44\x24\x00\x48\x8D\x54\x24\x00\x48\x8B\xCD\xE8\x00\x00\x00\x00\x48\x8B\xF0\x48\x89\x37")
    SDK_API DeferredRenderingRenderChunkSorter(
        std::shared_ptr<RenderChunkSorterSharedInfo> sharedInfo,
        std::shared_ptr<RenderChunkSorterSharedInfo>,
        std::shared_ptr<RenderChunkSorterSharedInfo>
    );
};