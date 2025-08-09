#include "UniformInt.h"

#if MC_VERSION >= v1_21_50

int UniformInt::sample(IRandom &rng) const {
    using Hook = sapphire::ApiLoader<
        "\x40\x53\x48\x83\xEC\x00\x48\x8B\x02\x4C\x8B\xC2\x8B\x51\x00\x48\x8B\xD9\x2B\x51"_sig,
        &UniformInt::sample>;
    return (this->*Hook::origin)(rng);
    /// vvv
    return this->mMin + rng.nextInt(this->mMax - this->mMin + 1);
}

#endif