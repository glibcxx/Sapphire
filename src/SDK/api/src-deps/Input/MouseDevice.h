#pragma once

#include "SDK/core/ApiManager.h"
#include "MouseAction.h"

// size: 56
class MouseDevice {
public:
    static constexpr int DELTA_NOTSET = -9999;

    int                      _index = -1;            // off+0
    short                    _x = 0;                 // off+4
    short                    _y = 0;                 // off+6
    short                    _dx = DELTA_NOTSET;     // off+8
    short                    _dy = DELTA_NOTSET;     // off+10
    short                    _xOld = 0;              // off+12
    short                    _yOld = 0;              // off+14
    char                     _buttonStates[5]{};     // off+16
    std::vector<MouseAction> _inputs{};              // off+24
    int                      _firstMovementType = 0; // off+48

    SDK_API void feed(
        char  actionButtonId,
        char  buttonData,
        short x,
        short y,
        short dx,
        short dy,
        bool  forceMotionlessPointer
    );
    MARK_HOOKABLE(&MouseDevice::feed)
};
static_assert(sizeof(MouseDevice) == 56);