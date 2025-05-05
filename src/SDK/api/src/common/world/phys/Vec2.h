#pragma once

#include "SDK/core/Core.h"

class Vec2 {
public:
    float x, y;

    std::string toString() const {
        return std::format("[{:.4f}, {:.4f}]", x, y);
    }
};