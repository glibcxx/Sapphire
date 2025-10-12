#include "MovingBlockActor.h"

Vec3 MovingBlockActor::getDrawPos(const class IConstBlockSource &region, float alpha) const {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xDA\x0F\x29\x7C\x24"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x00\x00\x00\x00\x00\x00\x00\x49\x8B\xF0\x0F\x29\x7C\x24"_sig,
#endif
        &MovingBlockActor::getDrawPos>;
    return (this->*Hook::origin)(region, alpha);
}

void MovingBlockActor::moveCollidedEntities(PistonBlockActor &pistonBlock, BlockSource &region) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x44\x8B\x45\x00\x48\x83\xC3\x00\xE9\x00\x00\x00\x00\xF3\x0F\x10\x8F"_sig,
        &MovingBlockActor::moveCollidedEntities>;
    return (this->*Hook::origin)(pistonBlock, region);
}