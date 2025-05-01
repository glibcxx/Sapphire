#pragma once

#include "SDK/core/Core.h"

class Vec3 {
public:
    float x, y, z;

    std::string toString() const {
        return std::format("[{:.4f}, {:.4f}, {:.4f}]", x, y, z);
    }
};