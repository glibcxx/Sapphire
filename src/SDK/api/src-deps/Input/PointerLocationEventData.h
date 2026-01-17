#pragma once

#include "SDK/api/src-deps/Input/InputMode.h"

// size: 12
struct PointerLocationEventData {
public:
    InputMode inputMode;              // off+0
    int16_t   x;                      // off+4
    int16_t   y;                      // off+6
    bool      forceMotionlessPointer; // off+8
};