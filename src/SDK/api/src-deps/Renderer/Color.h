#pragma once

#include "SDK/api/src-deps/Core/Math/Color.h"

// size: 20
class ShaderColor {
public:
    mce::Color color; // off+0
    bool       dirty; // off+16
};
static_assert(sizeof(ShaderColor) == 20);