#include "TextureResourceManager.h"

namespace dragon::rendering {

    TextureResourceManager *TextureResourceManager::ctor(GraphicsTasks &graphicsTasks) {
        using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x49\x8D\x6F"_sig,
#elif MC_VERSION == v1_21_50
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x4C\x8D\x67\x00\x4C\x89\xA4\x24"_sig,
#elif MC_VERSION == v1_21_60
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x4C\x8D\x7B\x00\x4C\x89\x7C\x24\x00\xE8\x00\x00\x00\x00\x48\x8B\xC8\x48\x8B\x00\xBA\x00\x00\x00\x00\x48\x8B\x40\x00\xFF\x15\x00\x00\x00\x00\x48\x8B\xF8\x48\x8D\x05"_sig,
#endif
            &TextureResourceManager::ctor>;
        return (this->*Bind::origin)(graphicsTasks);
    }

} // namespace dragon::rendering