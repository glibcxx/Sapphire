#pragma once

#include "MouseAction.h"

#include "SDK/core/ApiManager.h"

// size: 56
class MouseDevice {
public:
    int                      _index;             // off+0
    short                    _x;                 // off+4
    short                    _y;                 // off+6
    short                    _dx;                // off+8
    short                    _dy;                // off+10
    short                    _xOld;              // off+12
    short                    _yOld;              // off+14
    char                     _buttonStates[5];   // off+16
    std::vector<MouseAction> _inputs;            // off+24
    int                      _firstMovementType; // off+48

    SDK_API void feed(
        char  actionButtonId,
        int   buttonData,
        short x,
        short y,
        short dx,
        short dy,
        bool  forceMotionlessPointer
    );
};
static_assert(sizeof(MouseDevice) == 56);