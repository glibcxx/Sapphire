#pragma once

#include "FrameBuilder.h"
#include "BgfxBridge/BgfxFrameExtractor.h"
#include "SDK/api/src-external/RenderDragon/Platform/BgfxContext.h"

namespace mce::framebuilder {

    // size: 1344 (1.21.50)
    class BgfxFrameBuilder : public FrameBuilder {
    public:
        bool                                            mInitialized; // off+24
        std::unique_ptr<bgfxbridge::BgfxFrameExtractor> mExtractor;   // off+32
        std::unique_ptr<dragon::platform::BgfxContext>  mBgfxContext; // off+40
    };

} // namespace mce::framebuilder