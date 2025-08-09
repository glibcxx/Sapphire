#pragma once

#include <cstdint>

// size: 4
class SubChunkBlockPos {
    union /*`anonymous`*/
    {
        struct /*`anonymous`*/
        {
            uint8_t x; // off+0
            uint8_t y; // off+1
            uint8_t z; // off+2
        };
        uint32_t packed;
    };
};