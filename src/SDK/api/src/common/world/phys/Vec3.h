#pragma once

#include <glm/glm.hpp>
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
    constexpr Vec3(const glm::tvec3<float> &glm) :
        x(glm.x), y(glm.y), z(glm.z) {}

    constexpr Vec3 operator+(const Vec3 &other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    constexpr Vec3 operator+(float s) const {
        return Vec3(x + s, y + s, z + s);
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

    constexpr Vec3 &operator+=(const Vec3 &other) {
        x += other.x, y += other.y, z += other.z;
        return *this;
    }

    constexpr Vec3 &operator+=(int s) {
        x += s, y += s, z += s;
        return *this;
    }

    constexpr Vec3 &operator-=(const Vec3 &other) {
        x -= other.x, y -= other.y, z -= other.z;
        return *this;
    }

    constexpr Vec3 &operator-=(int s) {
        x -= s, y -= s, z -= s;
        return *this;
    }

    constexpr Vec3 &operator*=(const Vec3 &other) {
        x *= other.x, y *= other.y, z *= other.z;
        return *this;
    }

    constexpr operator glm::vec3() const {
        return {x, y, z};
    }

    static constexpr Vec3 min(const Vec3 &a, const Vec3 &b) {
        return {std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)};
    }

    static constexpr Vec3 max(const Vec3 &a, const Vec3 &b) {
        return {std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)};
    }

    std::string toString() const {
        return std::format("[{:.4f}, {:.4f}, {:.4f}]", x, y, z);
    }
};