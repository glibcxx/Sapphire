#pragma once

#include "FrameBuilder.h"
#include "BgfxBridge/BgfxFrameExtractor.h"
#include "SDK/api/src-external/RenderDragon/FrameObject/FrameRenderer.h"
#include "SDK/api/src-external/RenderDragon/Platform/BgfxContext.h"
#include "SDK/api/src-external/RenderDragon/Task/GraphicsTasks.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonTextureResourceService.h"
#include "SDK/api/src-external/RenderDragon/Rendering/BgfxPrimitiveIndexBuffer.h"
#include "SDK/api/src-client/common/client/renderer/chunks/RenderChunkGeometry.h"
#include "SDK/api/src-deps/Core/Memory/Pool.h"
#include "SDK/api/src-external/bgfx/bgfx.h"
#include "BgfxBridge/UITarget.h"

struct RenderChunkDirectVertexData;
namespace dragon::rendering {
    struct Camera;
}
namespace mce {
    class VertexFormat;
}

namespace mce::framebuilder {

    class FrameBuilderContext;

    // size: 1344 (1.21.50), todo: 1.21.2/1.21.60
    class BgfxFrameBuilder : public FrameBuilder {
    public:
        bool                                                                            mInitialized;                          // off+24
        std::unique_ptr<bgfxbridge::BgfxFrameExtractor>                                 mExtractor;                            // off+32
        std::unique_ptr<dragon::platform::BgfxContext>                                  mBgfxContext;                          // off+40
        Core::RingAllocatorContainer<uint8_t>                                           mIntraFrameAllocatorContainer;         // off+48
        Core::CpuRingBufferAllocator<uint8_t>                                          *mIntraFrameAllocator;                  // off+248
        std::unique_ptr<Core::RingAllocatorContainer<uint8_t>::AllocationScope>         mNextAllocationScope;                  // off+256
        dragon::task::GraphicsTasks                                                     mGraphicsTasks;                        // off+264
        std::unique_ptr<dragon::task::AsyncTasksScope>                                  mFrameAsyncTasks;                      // off+600
        std::unique_ptr<dragon::rendering::BgfxPrimitiveIndexBuffer>                    mQuadIndexBuffer;                      // off+608
        std::unique_ptr<dragon::rendering::BgfxPrimitiveIndexBuffer>                    mSequenceIndexBuffer;                  // off+616
        uint8_t                                                                         _fill624[212];                         // off+624
        dragon::rendering::LightingModels                                               mLightingModel;                        // off+836
        uint8_t                                                                         mUnk840;                               // off+840
        uint8_t                                                                         _fill841[31];                          // off+841
        std::shared_ptr<dragon::frameobject::GameFrame>                                 mTransactionConsumerFrame;             // off+872
        uint8_t                                                                         _fill888[64];                          // off+888
        std::string                                                                     mUnk952;                               // off+952
        dragon::TextureResourceService                                                  mInternalDragonTextureResourceService; // off+984
        dragon::ClientTexture                                                           mMissingTexture;                       // off+1056
        uint64_t                                                                        mFrameCounter;                         // off+1080
        std::map<uint8_t, glm::tvec3<float>>                                            mPreviousFramePlayerViewWorldOrigins;  // off+1088
        std::queue<std::pair<int64_t, std::shared_ptr<dragon::frameobject::GameFrame>>> mInFlightFrames;                       // off+1104
        std::unique_ptr<Pool<dragon::frameobject::GameFrame>>                           mFramePool;                            // off+1144
        uint8_t                                                                         mUnk1152;                              // off+1152
        bool                                                                            mEnableVrMode;                         // off+1153
        glm::tvec2<uint16_t>                                                            mResolution;                           // off+1154
        glm::tvec2<uint16_t>                                                            mClientResolution;                     // off+1158
        bgfx::DLSSOptions                                                               mUpscalingOptions;                     // off+1164
        uint64_t                                                                        mUnk1184;                              // off+1184
        double                                                                          mPreviousFrameStall;                   // off+1192
        bool                                                                            m_enabledMsaa;                         // off+1200
        std::vector<dragon::rendering::Camera>                                          mDebugCameras;                         // off+1208
        std::optional<bgfxbridge::UITarget>                                             mPersistentUiView;                     // off+1232
        std::optional<bgfxbridge::UITarget>                                             mUnk1288;                              // off+1288

        SDK_API BgfxFrameBuilder(
            WorkerPool                                   &rendererWorkerPool,
            gsl::span<std::reference_wrapper<WorkerPool>> helperPools,
            Scheduler                                    &scheduler
        );
#pragma SPHR_LINKER_SYM_ALIAS(                                                                                                                         \
    "??0BgfxFrameBuilder@framebuilder@mce@@QEAA@AEAVWorkerPool@@V?$span@V?$reference_wrapper@VWorkerPool@@@std@@$0?0@gsl@@AEAVScheduler@@@Z",          \
    "?ctor@BgfxFrameBuilder@framebuilder@mce@@QEAAPEAV123@AEAVWorkerPool@@V?$span@V?$reference_wrapper@VWorkerPool@@@std@@$0?0@gsl@@AEAVScheduler@@@Z" \
)

        SAPPHIRE_API("1.21.2", "\x4C\x8B\xDC\x49\x89\x5B\x00\x49\x89\x4B\x00\x55\x56\x57\x41\x54")
        SAPPHIRE_API("1.21.50", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x49\x8B\xE9\x4D\x8B\xE0\x4C\x8B\xFA\x48\x8B\xF9")
        SAPPHIRE_API("1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x00\x49\x8B\xF9\x49\x8B\xD8")
        SDK_API BgfxFrameBuilder *ctor(
            WorkerPool                                   &rendererWorkerPool,
            gsl::span<std::reference_wrapper<WorkerPool>> helperPools,
            Scheduler                                    &scheduler
        );

        SAPPHIRE_API("1.21.2", "\x40\x56\x41\x56\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x48\x8B\xF1")
        SAPPHIRE_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x48\x8B\xF1\x48\x81\xC1")
        SDK_API /*virtual*/ void destroy() /*override*/;

        SAPPHIRE_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x55\x57\x41\x54\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x48\x8B\xD9\x45\x33\xF6")
        SAPPHIRE_API("1.21.50", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x48\x8B\xF9\x45\x33\xED\x44\x89\x6C\x24\x00\x48\xFF\x81")
        SAPPHIRE_API("1.21.60", "\x48\x8B\xC4\x48\x89\x58\x00\x48\x89\x70\x00\x48\x89\x78\x00\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x48\x8B\xF9\x45\x33\xED")
        SDK_API /*virtual*/ void startFrame() /*override*/;

        SAPPHIRE_API("1.21.2", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x2B\xE0\x0F\x29\xB4\x24\x00\x00\x00\x00\x0F\x29\xBC\x24\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x48\x8B\xF2\x48\x89\x55")
        SAPPHIRE_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x2B\xE0\x0F\x29\xB4\x24\x00\x00\x00\x00\x0F\x29\xBC\x24\x00\x00\x00\x00\x44\x0F\x29\x84\x24\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4C\x8B")
        SDK_API /*virtual*/ void endFrame(mce::framebuilder::FrameBuilderContext &&frameBuilderContext) /*override*/;

        SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "\x48\x81\xEC\x00\x00\x00\x00\x44\x8B\x15")
        SDK_API /*virtual*/ void generateRenderChunkVertexData(
            RenderChunkDirectVertexData        &renderChunkDirectVertexData,
            const std::array<RangeIndices, 18> &layerRanges,
            const gsl::span<const uint8_t>     &chunkData,
            const uint64_t                     &vertexCount,
            const mce::VertexFormat            &mceFormat,
            const int                          *absoluteBlockPosition
        ) /*override*/;

        // size: 104
        class lambda_at_mce__framebuilder__BgfxFrameBuilder__endFrame {
        public:
            Core::CpuRingBufferAllocator<uint8_t>                  *allocator;          // off+0
            std::shared_ptr<dragon::frameobject::GameFrame>         frame;              // off+8
            std::unique_ptr<void>                                   context;            // off+24
            std::shared_ptr<void>                                   renderer;           // off+32
            std::optional<dragon::frameobject::FrameExplicitTarget> targetImageWrapper; // off+48

            SAPPHIRE_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x55\x57\x41\x56\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x48\x8B\xF1\x0F\x57\xC0\x0F\x11\x45")
            SAPPHIRE_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x55\x57\x41\x54\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x48\x8B\xD9\x45\x33\xE4\x0F\x57\xC0")
            SDK_API void operator()() const;
        };
        static_assert(sizeof(lambda_at_mce__framebuilder__BgfxFrameBuilder__endFrame) == 104);
    };
    static_assert(sizeof(BgfxFrameBuilder) == 1344);

} // namespace mce::framebuilder