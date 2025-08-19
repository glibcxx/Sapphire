#pragma once

#include <imgui.h>
#include "SDK/api/src/common/world/phys/Vec2.h"
#include "SDK/api/src-deps/Core/Math/Color.h"

namespace sapphire::ui {

    class Vec2 {
    public:
        float x = 0.0f, y = 0.0f;

        constexpr Vec2() = default;
        constexpr Vec2(const ::Vec2 &vec2) :
            x{vec2.x}, y{vec2.y} {}
        constexpr Vec2(const ImVec2 &vec2) :
            x{vec2.x}, y{vec2.y} {}
        constexpr Vec2(float x, float y) :
            x{x}, y{y} {}

        constexpr Vec2 operator+(const Vec2 &other) const { return {x + other.x, y + other.y}; }
        constexpr Vec2 operator-(const Vec2 &other) const { return {x - other.x, y - other.y}; }

        constexpr operator ImVec2() const { return {x, y}; }
    };

    class Vec4 {
    public:
        float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

        constexpr Vec4() = default;
        constexpr Vec4(const ImVec4 &vec4) :
            x{vec4.x}, y{vec4.y}, z{vec4.z}, w{vec4.w} {}
        constexpr Vec4(float x, float y, float z, float w) :
            x{x}, y{y}, z{z}, w{w} {}

        constexpr operator ImVec4() const { return {x, y, z, w}; }
    };

    class Rect {
    public:
        Vec2 min{};
        Vec2 max{};

        constexpr Rect() = default;
        constexpr Rect(const Vec2 &min, const Vec2 &max) :
            min{min}, max{max} {}
        constexpr Rect(float x, float y, float z, float w) :
            min{x, y}, max{z, w} {}
    };

    class Color {
    public:
        float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;

        constexpr Color() = default;
        constexpr Color(const mce::Color &color) :
            r{color.r}, g{color.g}, b{color.b}, a{color.a} {}
        constexpr Color(const ImVec4 &color) :
            r{color.x}, g{color.y}, b{color.z}, a{color.w} {}
        constexpr Color(float r, float g, float b, float a = 1.0f) :
            r{r}, g{g}, b{b}, a{a} {}

        constexpr operator ImVec4() const { return {r, g, b, a}; }
        constexpr operator float *() { return &r; }
        constexpr operator uint32_t() const {
            return IM_COL32(
                static_cast<unsigned int>(r * 255),
                static_cast<unsigned int>(g * 255),
                static_cast<unsigned int>(b * 255),
                static_cast<unsigned int>(a * 255)
            );
        }
    };

} // namespace sapphire::ui