#pragma once

#include "SDK/api/src-deps/Core/Container/Blob.h"
#include "SDK/api/src-deps/Coregraphics/ImageDescription.h"

namespace cg {

    // size: 48
    class ImageBuffer {
        using Storage = mce::Blob;

        Storage          mStorage;          // off+0
        ImageDescription mImageDescription; // off+24
    };

} // namespace cg