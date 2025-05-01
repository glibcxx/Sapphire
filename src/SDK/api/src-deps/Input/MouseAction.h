#pragma once

#include <cstdint>

class MouseAction {
public:
    enum ActionType : int8_t {
        ActionMove = 0,
        ActionLeft = 1,
        ActionRight = 2,
        ActionMiddle = 3,
        ActionWheel = 4,
        ActionX1 = 5,
        ActionX2 = 6,
        ActionMoveRelative = 7,
        DataUp = 0,
        DataDown = 1,
    };
};