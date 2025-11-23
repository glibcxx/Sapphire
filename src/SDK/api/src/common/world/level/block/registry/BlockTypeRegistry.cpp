#include "BlockTypeRegistry.h"

const Block &BlockTypeRegistry::getDefaultBlockState(const HashedString &name) {
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\xF0\x48\x89\x45\x00\x48\x8B\xCB"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x48\x8B\xD9\x33\xFF\x0F\x57\xC0"_sig,
#endif
        &BlockTypeRegistry::getDefaultBlockState>;
    return Bind::origin(name);
}