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
    char    mUnk16[16];
    int64_t mUnk32;
    int64_t mUnk40;
    int32_t mUnk48;

    SDK_API void feed(
        MouseAction::ActionType action,
        int                     buttonData,
        short                   x,
        short                   y,
        short                   dx,
        short                   dy,
        bool                    forceMotionlessPointer
    );
};
static_assert(sizeof(MouseDevice) == 0x38);