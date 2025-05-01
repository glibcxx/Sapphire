#pragma once

#include "MouseAction.h"

#include "SDK/core/Core.h"

class MouseDevice {
public:
    char    gap0[4];
    short   x;
    short   y;
    short   dx;
    short   dy;
    short   xPrev;
    short   yPrev;
    char    mUnk10[16];
    int64_t mUnk20;
    int64_t mUnk28;
    int32_t mUnk30;

    SDK_API void feed(
        MouseAction::ActionType action,
        int                     buttonData,
        short                   x,
        short                   y,
        short                   dx,
        short                   dy,
        uint8_t                 a8
    );
};

static_assert(offsetof(MouseDevice, mUnk20) == 0x20);
static_assert(sizeof(MouseDevice) == 0x38);