#pragma once

#include "SDK/api/src-external/bgfx/bgfx.h"
#include "SDK/api/src-external/RenderDragon/Mesh/Mesh.h"

namespace dragon::platform {

    class ResourcesManager;

    // size: 120 (1.21.50)
    class BgfxContext {
        // size: 1
        struct DeviceLostInfo {};

        class LocalResources {
        public:
            dragon::platform::ResourcesManager &mResourceManager; // off+0
            dragon::mesh::Mesh                  mScreenQuad;      // off+8
        };

        std::unique_ptr<bgfx::CallbackI>                               mBgfxCallbacks;    // off+0
        std::unique_ptr<dragon::platform::BgfxContext::LocalResources> mLocalResources;   // off+8
        std::unique_ptr<dragon::platform::ResourcesManager>            mResourcesManager; // off+16

        // ...
    };

} // namespace dragon::platform