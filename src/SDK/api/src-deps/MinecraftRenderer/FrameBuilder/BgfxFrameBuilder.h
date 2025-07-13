#pragma once

#include "FrameBuilder.h"
#include "BgfxBridge/BgfxFrameExtractor.h"
#include "SDK/api/src-external/RenderDragon/FrameObject/FrameRenderer.h"
#include "SDK/api/src-external/RenderDragon/Platform/BgfxContext.h"
#include "SDK/api/src-external/RenderDragon/Task/GraphicsTasks.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonTextureResourceService.h"
#include "SDK/api/src-external/RenderDragon/Rendering/BgfxPrimitiveIndexBuffer.h"
#include "SDK/api/src-deps/Core/Memory/Pool.h"
#include "SDK/api/src-external/bgfx/bgfx.h"
#include "BgfxBridge/UITarget.h"

namespace dragon::rendering {
    struct Camera;
}

namespace mce::framebuilder {

    class FrameBuilderContext;

    // size: 1344 (1.21.50)
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
        uint32_t                                                                        mUnk836;                               // off+836
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
        uint16_t                                                                        mUnk1152;                              // off+1152
        glm::tvec2<uint16_t>                                                            mResolution;                           // off+1154
        glm::tvec2<uint16_t>                                                            mClientResolution;                     // off+1158
        bgfx::DLSSOptions                                                               mUpscalingOptions;                     // off+1164
        uint64_t                                                                        mUnk1184;                              // off+1184
        double                                                                          mPreviousFrameStall;                   // off+1192
        bool                                                                            m_enabledMsaa;                         // off+1200
        std::vector<dragon::rendering::Camera>                                          mDebugCameras;                         // off+1208
        std::optional<bgfxbridge::UITarget>                                             mPersistentUiView;                     // off+1232
        std::optional<bgfxbridge::UITarget>                                             mUnk1288;                              // off+1288

        SDK_API BgfxFrameBuilder *ctor(
            WorkerPool                                   &rendererWorkerPool,
            gsl::span<std::reference_wrapper<WorkerPool>> helperPools,
            Scheduler                                    &scheduler
        );

        SDK_API /*virtual*/ void destroy() /*override*/;

        SDK_API /*virtual*/ void endFrame(mce::framebuilder::FrameBuilderContext &&frameBuilderContext) /*override*/;

        // size: 104
        class lambda_at_mce__framebuilder__BgfxFrameBuilder__endFrame {
        public:
            Core::CpuRingBufferAllocator<uint8_t>                  *allocator;          // off+0
            std::shared_ptr<dragon::frameobject::GameFrame>         frame;              // off+8
            std::unique_ptr<void>                                   context;            // off+24
            std::shared_ptr<void>                                   renderer;           // off+32
            std::optional<dragon::frameobject::FrameExplicitTarget> targetImageWrapper; // off+48

            SDK_API void operator()() const;
        };
        static_assert(sizeof(lambda_at_mce__framebuilder__BgfxFrameBuilder__endFrame) == 104);
    };
    static_assert(sizeof(BgfxFrameBuilder) == 1344);

} // namespace mce::framebuilder