#include "BlockTessellatorCache.h"

void BlockTessellatorCache::reset(BlockSource *region, const BlockPos &startPostion) {
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x89\x51\x00\x48\x8B\xD9"_sig,
        &BlockTessellatorCache::reset>;
    return (this->*Hook::origin)(region, startPostion);
}

BrightnessPair BlockTessellatorCache::getLightColor(const BlockPos &p, void *a2) {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x41\xBC\x00\x00\x00\x00\x33\xF6"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x45\x33\x00\x4C\x8D\x5C\x24"_sig,
#endif
        &BlockTessellatorCache::getLightColor>;
    return (this->*Hook::origin)(p, a2);
}