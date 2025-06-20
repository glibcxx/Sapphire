#pragma once

#include <glm/glm.hpp>
#include "SDK/api/src-external/RenderDragon/Resource/DragonTexture.h"

namespace DFC /*dragon::frameobject::components*/ {

    // size: 80 (1.21.50)
    struct RenderTarget {
        dragon::res::ServerTexture mColorTargetTextureHandle; // off+0
        dragon::res::ServerTexture mDepthTargetTextureHandle; // off+24
        glm::tvec4<float>          mViewport;                 // off+48
        glm::tvec4<float>          mScissor;                  // off+64
    };

} // namespace DFC