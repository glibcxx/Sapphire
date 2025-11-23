#pragma once

#include <functional>

#include "SDK/core/SymbolResolver.h"
#include "SDK/api/src-deps/Core/Utility/optional_ref.h"

namespace rendergraph {

    class Pass;
    class RenderContext;
    class Resource;

    // size: 16
    struct ResourceBinding {
        enum class AccessType : int32_t {
            UNSPECIFIED = 0,
            WAIT = 1,
            READ = 2,
            WRITE = 3,
            CREATE = 4,
        };

        enum class ResourceType : int32_t {
            UNSPECIFIED = 0,
            TEXTURE = 1,
            BUFFER = 2,
        };

        enum class ShaderStage : int32_t {
            UNSPECIFIED = 0,
            PIXEL_SHADER = 1,
            NON_PIXEL_SHADER = 2,
        };

        AccessType   mAccessType;   // off+0
        ResourceType mResourceType; // off+4
        ShaderStage  mShaderStage;  // off+8
        int32_t      mIndex;        // off+12
    };
    static_assert(sizeof(ResourceBinding) == 16);

    // size: 40
    struct ResourceEvent {
        Resource          &mResource;            // off+0
        optional_ref<Pass> mPass;                // off+8
        ResourceBinding    mBinding;             // off+16
        int32_t            mNextEventIndex = -1; // off+32
    };
    static_assert(sizeof(ResourceEvent) == 40);

} // namespace rendergraph