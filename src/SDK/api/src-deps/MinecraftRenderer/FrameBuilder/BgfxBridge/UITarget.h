#pragma once

#include "SDK/api/src-external/RenderDragon/Resource/DragonTextureResourceService.h"

namespace mce::framebuilder::bgfxbridge {

    // size: 48
    struct UITarget {
        dragon::ServerTexture mColor; // off+0
        dragon::ServerTexture mDepth; // off+24
    };

} // namespace mce::framebuilder::bgfxbridge