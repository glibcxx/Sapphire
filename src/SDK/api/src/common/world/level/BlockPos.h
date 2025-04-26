#pragma once

#include <format>
#include "SDK/core/Core.h"

class BlockPos {
public:
    int x, y, z;

    std::string toString() const {
        return std::format("[{}, {}, {}]", x, y, z);
    }

    bool operator==(const BlockPos &other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};
