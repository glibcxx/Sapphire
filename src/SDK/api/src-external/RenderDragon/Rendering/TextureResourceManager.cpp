#include "TextureResourceManager.h"

namespace dragon::rendering {

    TextureResourceManager *TextureResourceManager::ctor(GraphicsTasks &graphicsTasks) {
        using Hook = core::ApiLoader<
            "\xE8\x00\x00\x00\x00\x90\x4C\x8D\x67\x00\x4C\x89\xA4\x24"_sig, // 1.21.50
            &TextureResourceManager::ctor,
            core::deRefCall>;
        return (this->*Hook::origin)(graphicsTasks);
    }

} // namespace dragon::rendering