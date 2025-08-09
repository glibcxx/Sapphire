#pragma once

class Dimension;

class BaseLightTextureImageBuilder {
public:
    const Dimension * mDimension;

    // vtb+0
    virtual ~BaseLightTextureImageBuilder() = default;
};