#pragma once

#include "Direction.h"
#include "SDK/api/src/common/world/phys/Vec3.h"

class Random;

class Facing {
public:
    enum Name {
        DOWN = 0,
        UP = 1,
        NORTH = 2,
        SOUTH = 3,
        WEST = 4,
        EAST = 5,
        MAX = 6,
        NOT_DEFINED = 6,
        NUM_CULLING_IDS = 7,
    };

    enum class Rotation {
        NONE = 0,
        CCW = 1,
        OPP = 2,
        CW = 3,
    };

    static constexpr Name DIRECTIONS[6] = {
        Facing::DOWN,
        Facing::UP,
        Facing::NORTH,
        Facing::SOUTH,
        Facing::WEST,
        Facing::EAST,
    };
    static constexpr BlockPos DIRECTION[6];
    static constexpr Vec3     NORMAL[6] = {
        {0.0f,  -1.0f, 0.0f },
        {0.0f,  1.0f,  0.0f },
        {0.0f,  0.0f,  1.0f },
        {0.0f,  0.0f,  -1.0f},
        {-1.0f, 0.0f,  0.0f },
        {1.0f,  0.0f,  0.0f },
    };
    static constexpr Name BY2DDATA[4] = {
        Facing::NORTH,
        Facing::SOUTH,
        Facing::WEST,
        Facing::EAST,
    };
    static constexpr uint8_t                                FACINGMASK[6] = {0x1, 0x2, 0x4, 0x8, 0x10, 0x20};
    static constexpr uint8_t                                FACINGMASK_INV[6] = {0x3E, 0x3D, 0x3B, 0x37, 0x2F, 0x1F};
    static constexpr std::array<FacingID, 6>                ALL_FACES = {1, 0, 2, 3, 4, 5};
    static constexpr std::array<std::array<FacingID, 6>, 6> ALL_EXCEPT = {
        {
         {1, 2, 3, 4, 5},
         {0, 2, 3, 4, 5},
         {1, 0, 3, 4, 5},
         {1, 0, 2, 4, 5},
         {1, 0, 2, 3, 5},
         {1, 0, 2, 3, 4},
         }
    };
    ;
    static constexpr std::array<std::array<FacingID, 3>, 6> ALL_EXCEPT_AXIS_Y = {
        {
         {},
         {},
         {3, 4, 5},
         {2, 4, 5},
         {2, 3, 5},
         {2, 3, 4},
         }
    };

    class Plane {
    public:
        static constexpr std::array<FacingID, 4> HORIZONTAL = {2, 5, 3, 4};
    };
};