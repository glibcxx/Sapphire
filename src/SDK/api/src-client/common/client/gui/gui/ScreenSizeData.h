#pragma once

#include "SDK/api/src/common/world/phys/Vec2.h"

// size: 24
struct ScreenSizeData {
    Vec2 totalScreenSize;    // off+0
    Vec2 clientScreenSize;   // off+8
    Vec2 clientUIScreenSize; // off+16
};