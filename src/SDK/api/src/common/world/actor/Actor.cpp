#include "Actor.h"

const ActorUniqueID &Actor::getOrCreateUniqueID() const {
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        "\x40\x53\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x8B\x41\x00\x48\x8D\x54\x24"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x40\x53\x48\x83\xEC\x00\x4C\x8B\x51\x00\xBB\x00\x00\x00\x00\x8B\x51"_sig,
#endif
        &Actor::getOrCreateUniqueID>;
    return (this->*Bind::origin)();
}