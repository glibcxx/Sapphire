#pragma once

#include <glm/glm.hpp>
#include "Vec3.h"

class Vec4 {
public:
    float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

    constexpr Vec4() {}
    constexpr Vec4(float a) :
        x(a), y(a), z(a), w(a) {}
    constexpr Vec4(float x, float y, float z, float w) :
        x(x), y(y), z(z), w(w) {}
    constexpr Vec4(const glm::tvec4<float> &glm) :
        x(glm.x), y(glm.y), z(glm.z), w(glm.w) {}

    constexpr Vec4 &operator=(const Vec4 &other) {
        x = other.x, y = other.y, z = other.z, w = other.w;
        return *this;
    }

    constexpr Vec4 operator+(const Vec4 &other) const {
        return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    constexpr Vec4 operator+(float s) const {
        return Vec4(x + s, y + s, z + s, w + s);
    }

    constexpr Vec4 operator-(const Vec4 &other) const {
        return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    constexpr Vec4 operator-() const {
        return Vec4(-x, -y, -z, -w);
    }

    constexpr Vec4 operator*(const Vec4 &other) const {
        return Vec4(x * other.x, y * other.y, z * other.z, w * other.w);
    }

    constexpr Vec4 operator*(float s) const {
        return Vec4(x * s, y * s, z * s, w * s);
    }

    constexpr Vec4 &operator+=(const Vec4 &other) {
        x += other.x, y += other.y, z += other.z, w += other.w;
        return *this;
    }

    constexpr Vec4 &operator+=(float s) {
        x += s, y += s, z += s, w += s;
        return *this;
    }

    constexpr Vec4 &operator-=(const Vec4 &other) {
        x -= other.x, y -= other.y, z -= other.z, w -= other.w;
        return *this;
    }

    constexpr Vec4 &operator-=(float s) {
        x -= s, y -= s, z -= s, w -= s;
        return *this;
    }

    constexpr Vec4 &operator*=(const Vec4 &other) {
        x *= other.x, y *= other.y, z *= other.z, w *= other.w;
        return *this;
    }

    constexpr Vec4 &operator*=(float s) {
        x *= s, y *= s, z *= s, w *= s;
        return *this;
    }

    operator glm::vec4() const {
        return {x, y, z, w};
    }

    std::string toString() const {
        return std::format("[{:.4f}, {:.4f}, {:.4f}, {:.4f}]", x, y, z, w);
    }
};