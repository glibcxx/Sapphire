#pragma once

#include "SDK/api/src-external/RenderDragon/Resource/DragonTextureResourceService.h"

namespace mce::framebuilder {

    struct BlitFlipbookSingleTextureDescription;

    struct BlitFlipbookTextureDescription {
        const dragon::ServerTexture                           mRenderTargetTexture; // off+0
        gsl::span<const BlitFlipbookSingleTextureDescription> mTextures;            // off+24
    };

} // namespace mce::framebuilder