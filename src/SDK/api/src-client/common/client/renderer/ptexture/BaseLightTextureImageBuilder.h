#pragma once

#include "pch.h" // IWYU pragma: keep

class Dimension;

class BaseLightTextureImageBuilder {
public:
    const Dimension * mDimension;

    // vtb+0
    virtual ~BaseLightTextureImageBuilder() = default;
};