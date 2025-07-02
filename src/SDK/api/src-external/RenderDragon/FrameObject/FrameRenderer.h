#pragma once

#include "SDK/api/src-external/RenderDragon/Resource/DragonTextureResourceService.h"

namespace dragon::frameobject {

    // size: 48 (1.21.50)
    struct FrameExplicitTarget {
        dragon::ServerTexture  mTexture; // off+0
        cg::TextureDescription mDesc;    // off+24
    };

} // namespace dragon::frameobject