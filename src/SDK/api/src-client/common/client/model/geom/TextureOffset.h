#pragma once

#include "pch.h" // IWYU pragma: keep

// size: 16
class TextureOffset {
public:
    int x; // off+0
    int y; // off+4

    // vtb+0
    virtual ~TextureOffset() = default;
};