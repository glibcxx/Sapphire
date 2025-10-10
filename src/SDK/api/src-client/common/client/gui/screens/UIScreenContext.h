#pragma once

#include <cstdint>

// size: 12
class UIScreenContext {
public:
    const int32_t xm; // off+0
    const int32_t ym; // off+4
    const float   a;  // off+8
};
static_assert(sizeof(UIScreenContext) == 12);