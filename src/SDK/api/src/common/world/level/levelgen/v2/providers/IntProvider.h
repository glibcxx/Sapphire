#pragma once

#if MC_VERSION >= v1_21_50

#    include <variant>
#    include "SDK/core/ApiManager.h"
#    include "ConstantInt.h"
#    include "UniformInt.h"

// size: 32
struct IntProvider : public IntProviderType {
    std::variant<ConstantInt, UniformInt> mInt; // off+8

    // \x48\x83\xEC\x00\x48\x0F\xBE\x41\x18\x48\x83\xC1\x00\x48\x83\xC0\x00\x74\x00\x48\x8B\x01\x48\x8B\x40\x08\xFF\x15\x00\x00\x00\x00\x48\x83\xC4
    virtual int sample(IRandom &rng) const override {
        if (mInt.index() == 0)
            return std::get<ConstantInt>(mInt).sample(rng);
        else
            return std::get<UniformInt>(mInt).sample(rng);
    }

    virtual int maxValue() const override {
        if (mInt.index() == 0)
            return std::get<ConstantInt>(mInt).maxValue();
        else
            return std::get<UniformInt>(mInt).maxValue();
    }

    virtual int minValue() const override {
        if (mInt.index() == 0)
            return std::get<ConstantInt>(mInt).minValue();
        else
            return std::get<UniformInt>(mInt).minValue();
    }
};
static_assert(sizeof(IntProvider) == 32);

#endif