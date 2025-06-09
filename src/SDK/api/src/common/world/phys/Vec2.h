#pragma once

#include "SDK/core/Core.h"
#include <glm/glm.hpp>

class Vec2 {
public:
    float x, y;

    constexpr Vec2() {}
    constexpr Vec2(float a) :
        x(a), y(a) {}
    constexpr Vec2(float x, float y) :
        x(x), y(y) {}
    constexpr Vec2(const glm::tvec2<float> &glm) :
        x(glm.x), y(glm.y) {}

    constexpr Vec2 operator+(const Vec2 &other) const {
        return Vec2(x + other.x, y + other.y);
    }

    constexpr Vec2 operator+(float s) const {
        return Vec2(x + s, y + s);
    }

    constexpr Vec2 operator-(const Vec2 &other) const {
        return Vec2(x - other.x, y - other.y);
    }

    constexpr Vec2 operator-() const {
        return Vec2(-x, -y);
    }

    constexpr Vec2 operator*(const Vec2 &other) const {
        return Vec2(x * other.x, y * other.y);
    }

    constexpr Vec2 &operator+=(const Vec2 &other) {
        x += other.x, y += other.y;
        return *this;
    }

    constexpr Vec2 &operator+=(int s) {
        x += s, y += s;
        return *this;
    }

    constexpr Vec2 &operator-=(const Vec2 &other) {
        x -= other.x, y -= other.y;
        return *this;
    }

    constexpr Vec2 &operator-=(int s) {
        x -= s, y -= s;
        return *this;
    }

    constexpr Vec2 &operator*=(const Vec2 &other) {
        x *= other.x, y *= other.y;
        return *this;
    }

    std::string toString() const {
        return std::format("[{:.4f}, {:.4f}]", x, y);
    }
};