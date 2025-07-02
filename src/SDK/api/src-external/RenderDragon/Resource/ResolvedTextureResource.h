#pragma once

#include <optional>
#include <functional>
#include "SDK/api/src-external/bgfx/bgfx.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonTextureDescription.h"
#include "SDK/api/src-deps/Core/Threading/MPMCQueue.h"
#include "SDK/api/src-deps/Coregraphics/TextureMetaData.h"

namespace dragon {

    // size: 128 (1.21.50)
    struct ResolvedTextureResource {
        bgfx::TextureHandle                     mTextureHandle;      // off+0
        dragon::TextureDescription              mTextureDescription; // off+8
        std::optional<cg::TextureMetaData>      mTextureMetaData;    // off+72
        mpmc::Sender<std::function<void(void)>> mDestructionQueue;   // off+112

        void dtor() noexcept; // \x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xD9\x0F\xB7\x01 (1.21.50)
    };
    static_assert(sizeof(ResolvedTextureResource) == 128);

} // namespace dragon