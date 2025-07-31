#pragma once

#include <cstdint>

// size: 20
class MouseAction {
public:
    enum ActionType : int8_t {
        ACTION_MOVE = 0,
        ACTION_LEFT = 1,
        ACTION_RIGHT = 2,
        ACTION_MIDDLE = 3,
        ACTION_WHEEL = 4,
        ACTION_X1 = 5,
        ACTION_X2 = 6,
        ACTION_MOVE_RELATIVE = 7,
        DATA_UP = 0,
        DATA_DOWN = 1,
    };

    short x;                      // off+0
    short y;                      // off+2
    short dx;                     // off+4
    short dy;                     // off+6
    char  action;                 // off+8
    char  data;                   // off+9
    int   pointerId;              // off+12
    bool  forceMotionlessPointer; // off+16
};