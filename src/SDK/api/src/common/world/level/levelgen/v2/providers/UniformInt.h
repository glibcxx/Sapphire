#pragma once

#if MC_VERSION >= v1_21_50

#    include "SDK/core/SymbolResolver.h"
#    include "IntProviderType.h"

// size: 16
struct UniformInt : public IntProviderType {
    int mMin; // off+8
    int mMax; // off+12

    // vtb+1
    SDK_API virtual int sample(IRandom &rng) const override;

    // vtb+2
    virtual int maxValue() const override { return this->mMax; }

    // vtb+3
    virtual int minValue() const override { return this->mMin; }
};

#endif
