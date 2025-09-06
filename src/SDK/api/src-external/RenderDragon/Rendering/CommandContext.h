#pragma once

#include "SDK/api/src-deps/Core/Memory/CpuRingBufferAllocator.h"
#include "SDK/api/src-external/RenderDragon/Materials/MaterialUniformName.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonTextureResourceService.h"

namespace bgfx {
    struct Encoder;
}

namespace dragon {

    namespace platform {
        class ResourcesManager;
    }

    namespace rendering {

        // size: 64
        class CommandContext {
        public:
            Core::CpuRingBufferAllocator<unsigned char> &mAllocator;        // off+0
            const uint16_t                               mViewId;           // off+8
            bgfx::Encoder *const                         mEncoder;          // off+16
            dragon::platform::ResourcesManager          &mResourcesManager; // off+24
            dragon::ResolvedTextureResource             *mMissingTexture;   // off+32

            // size: 32
            struct MaterialTextureBinding {
                dragon::materials::MaterialUniformName nameHash;      // off+0
                dragon::ServerTexture                  textureHandle; // off+8
                uint32_t                               textureFlags;  // off+24
            };

            std::vector<MaterialTextureBinding> mMaterialTextureBindings; // off+40
        };
    } // namespace rendering

} // namespace dragon