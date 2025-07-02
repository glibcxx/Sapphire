#pragma once

#include <vector>
#include "SDK/api/src-external/RenderDragon/Resource/DragonTextureDescription.h"
#include "SDK/api/src-deps/Coregraphics/ImageBuffer.h"

namespace dragon::res {

    // size: 88 (1.21.50)
    struct CreateTextureTransaction {
        // .?AVCreateTextureTransaction@res@dragon@@

        dragon::TextureDescription   mTextureDescription; // off+0
        std::vector<cg::ImageBuffer> mImageBuffers;       // off+64
    };

    // size: 56 (1.21.50)
    struct CreateReadbackTextureTransaction {
        // .?AVCreateReadbackTextureTransaction@res@dragon@@

        cg::TextureDescription mDescription; // off+0
        std::string            mName;        // off+24
    };

} // namespace dragon::res