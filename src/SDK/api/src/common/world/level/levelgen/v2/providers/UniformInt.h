#pragma once

#if MC_VERSION >= v1_21_50

#    include "SDK/core/SymbolResolver.h"
#    include "IntProviderType.h"

// size: 16
struct UniformInt : public IntProviderType {
    int mMin; // off+8
    int mMax; // off+12

    // vtb+1
    SAPPHIRE_API("1.21.50,1.21.60", "\x40\x53\x48\x83\xEC\x00\x48\x8B\x02\x4C\x8B\xC2\x8B\x51\x00\x48\x8B\xD9\x2B\x51")
    SDK_API virtual int sample(IRandom &rng) const override;

    // vtb+2
    virtual int maxValue() const override { return this->mMax; }

    // vtb+3
    virtual int minValue() const override { return this->mMin; }
};

#endif
