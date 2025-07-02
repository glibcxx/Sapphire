#pragma once

#include "SDK/api/src-deps/Core/Threading/MPMCQueue.h"
#include "DragonTextureResourceService.h"

namespace dragon {

    class DragonFrameResources {
    public:
        MPMCQueue<dragon::ServerTexture> mTextures;               // off+0
        dragon::ResolvedTextureResource *mMissingTextureResource; // off+616
    };

} // namespace dragon