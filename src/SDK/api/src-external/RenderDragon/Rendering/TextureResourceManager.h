#pragma once

#include <optional>

#include "SDK/core/Core.h"
#include "BufferHandleType.h"

namespace dragon::rendering {

    class GraphicsTasks;

    // size: 32
    class TextureResourceManager {
    public:
        class Impl {};

        std::unique_ptr<Impl>                                                          mImpl;           // off+0
        std::optional<details::BufferHandleTypeWrapper<details::TextureHandleTypeKey>> mMissingTexture; // off+8
        GraphicsTasks                                                                 &mGraphicsTasks;  // off+24

        SDK_API TextureResourceManager *ctor(GraphicsTasks &graphicsTasks);
    };

} // namespace dragon::rendering