#pragma once

#include <cstdint>

typedef uint8_t FacingID;

class Direction {
public:
    enum class Type : unsigned char {
        UNDEFINED = (unsigned char)-1,
        SOUTH = 0,
        WEST = 1,
        NORTH = 2,
        EAST = 3,
    };
};
