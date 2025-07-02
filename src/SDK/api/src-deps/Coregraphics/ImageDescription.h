#pragma once

#include "Enums/TextureFormat.h"
#include "Enums/ColorSpace.h"

namespace cg {

    // size: 20 (1.21.50)
    struct ImageDescription {
        uint32_t           mWidth;         // off+0
        uint32_t           mHeight;        // off+4
        mce::TextureFormat mTextureFormat; // off+8
        cg::ColorSpace     mColorSpace;    // off+12
        bool               mIsCubemap;     // off+13
        uint32_t           mArraySize;     // off+56
    };

} // namespace cg