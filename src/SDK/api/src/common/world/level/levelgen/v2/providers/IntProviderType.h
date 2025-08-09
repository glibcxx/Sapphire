#pragma once

#if MC_VERSION >= v1_21_50

#    include "SDK/api/src-deps/Core/Math/IRandom.h"

// size: 8
struct IntProviderType {
    // vtb+0
    virtual ~IntProviderType() = default;

    // vtb+1
    virtual int sample(IRandom &rng) const = 0;

    // vtb+2
    virtual int maxValue() const = 0;

    // vtb+3
    virtual int minValue() const = 0;
};

#endif