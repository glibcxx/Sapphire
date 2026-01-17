#pragma once

// https://github.com/LiteLDev/LeviLamina/blob/08e88a0c098ce030fa5fbe35c18af45cd46fa794/src/mc/deps/input/enums/DirectionId.h#L5

enum class DirectionId : int {
    Move = 0,
    Turn = 1,
    Scroll = 2,
    TurnRaw = 3,
    TurnUp = 4,
    TurnDown = 5,
    TurnLeft = 6,
    TurnRight = 7,
    Count,
};
