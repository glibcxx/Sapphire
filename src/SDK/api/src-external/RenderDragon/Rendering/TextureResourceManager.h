#pragma once

#include <optional>

#include "SDK/core/SymbolResolver.h"
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

        SAPPHIRE_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x90\x49\x8D\x6F")
        SAPPHIRE_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x90\x4C\x8D\x67\x00\x4C\x89\xA4\x24")
        SAPPHIRE_API("1.21.60", "call", "\xE8\x00\x00\x00\x00\x90\x4C\x8D\x7B\x00\x4C\x89\x7C\x24\x00\xE8\x00\x00\x00\x00\x48\x8B\xC8\x48\x8B\x00\xBA\x00\x00\x00\x00\x48\x8B\x40\x00\xFF\x15\x00\x00\x00\x00\x48\x8B\xF8\x48\x8D\x05")
        SDK_API TextureResourceManager *ctor(GraphicsTasks &graphicsTasks);
    };

} // namespace dragon::rendering