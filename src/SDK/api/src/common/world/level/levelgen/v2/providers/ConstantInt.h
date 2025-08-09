#pragma once

#if MC_VERSION >= v1_21_50

#    include "IntProviderType.h"

// size: 16
struct ConstantInt : public IntProviderType {
    int mValue; // off+8

    // vtb+1
    virtual int sample(IRandom &rng) const override { return this->mValue; }

    // vtb+2
    virtual int maxValue() const override { return this->mValue; }

    // vtb+3
    virtual int minValue() const override { return this->mValue; }
};

#endif