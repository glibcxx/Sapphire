#pragma once

#include "FrameBuilder.h"
#include "BgfxBridge/BgfxFrameExtractor.h"
#include "SDK/api/src-external/RenderDragon/FrameObject/FrameRenderer.h"
#include "SDK/api/src-external/RenderDragon/Platform/BgfxContext.h"
#include "SDK/api/src-deps/Core/Memory/Pool.h"

namespace mce::framebuilder {

    class FrameBuilderContext;

    // size: 1344 (1.21.50)
    class BgfxFrameBuilder : public FrameBuilder {
    public:
        bool                                            mInitialized; // off+24
        std::unique_ptr<bgfxbridge::BgfxFrameExtractor> mExtractor;   // off+32
        std::unique_ptr<dragon::platform::BgfxContext>  mBgfxContext; // off+40

        // std::shared_ptr<dragon::frameobject::GameFrame> mTransactionConsumerFrame; // off+872

        // std::unique_ptr<Pool<dragon::frameobject::GameFrame>> mFramePool; // off+1144

        /*virtual*/ void endFrame(mce::framebuilder::FrameBuilderContext &&frameBuilderContext) /*override*/;

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

} // namespace mce::framebuilder