#pragma once

#include "pch.h" // IWYU pragma: keep

#include "ImageDescription.h"

namespace cg {

    // size: 24 (1.21.50)
    struct TextureDescription : public ImageDescription {
        uint32_t mMipCount; // off+20
    };

} // namespace cg