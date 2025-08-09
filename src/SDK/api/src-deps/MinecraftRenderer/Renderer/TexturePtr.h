#pragma once

#include <memory>
#include "SDK/api/src-deps/Core/Resource/ResourceHelper.h"

class BedrockTextureData;

namespace mce {

    // size: 32
    class TexturePtr {
    public:
        std::shared_ptr<const BedrockTextureData> mClientTexture;    // off+0
        std::shared_ptr<ResourceLocation>         mResourceLocation; // off+16
    };

} // namespace mce