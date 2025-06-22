#pragma once

#include <glm/glm.hpp>
#include "SDK/api/src-external/RenderDragon/Resource/DragonTextureResourceService.h"

namespace DFC /*dragon::frameobject::components*/ {

    // size: 80 (1.21.50)
    struct RenderTarget {
        dragon::ServerTexture mColorTargetTextureHandle; // off+0
        dragon::ServerTexture mDepthTargetTextureHandle; // off+24
        glm::tvec4<float>     mViewport;                 // off+48
        glm::tvec4<float>     mScissor;                  // off+64
    };

} // namespace DFC