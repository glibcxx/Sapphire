#include "MovingBlockActor.h"

Vec3 MovingBlockActor::getDrawPos(const class IConstBlockSource &region, float alpha) const {
#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x00\x00\x00\x00\x00\x00\x00\x49\x8B\xF0\x0F\x29\x7C\x24"_sig,
        &MovingBlockActor::getDrawPos>;
#elif MC_VERSION == v1_21_2
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xDA\x0F\x29\x7C\x24"_sig,
        &MovingBlockActor::getDrawPos>;
#endif
    return (this->*Hook::origin)(region, alpha);
}
