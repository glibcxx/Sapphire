#pragma once

// size: 72
class DimensionBrightnessRamp {
    float mBrightnessRamp[16]; // off+8

    // vtb+0
    virtual ~DimensionBrightnessRamp() = default;

    // vtb+1
    virtual void buildBrightnessRamp();

    // vtb+2
    virtual float getBaseAmbientValue() const;
};