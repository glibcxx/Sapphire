#pragma once

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

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "\x48\x8B\xC4\x48\x89\x58\x00\x48\x89\x68\x00\x48\x89\x70\x00\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x44\x0F\xB7\xBC\x24")
    SDK_API void feed(
        char  actionButtonId,
        char  buttonData,
        short x,
        short y,
        short dx,
        short dy,
        bool  forceMotionlessPointer
    );
};
static_assert(sizeof(MouseDevice) == 56);