#pragma once

#include "Vec2.h"
#include "SDK/api/src/common/world/level/BlockPos.h"

class Vec3 {
public:
    float x = 0.0f, y = 0.0f, z = 0.0f;

    constexpr Vec3() {}
    constexpr Vec3(float a) :
        x(a), y(a), z(a) {}
    constexpr Vec3(float x, float y, float z) :
        x(x), y(y), z(z) {}
    constexpr Vec3(const BlockPos &b) :
        x(b.x), y(b.y), z(b.z) {}

    constexpr Vec3 operator+(const Vec3 &other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    constexpr Vec3 operator-(const Vec3 &other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    constexpr Vec3 operator-() const {
        return Vec3(-x, -y, -z);
    }

    constexpr Vec3 operator*(const Vec3 &other) const {
        return Vec3(x * other.x, y * other.y, z * other.z);
    }

    std::string toString() const {
        return std::format("[{:.4f}, {:.4f}, {:.4f}]", x, y, z);
    }
};