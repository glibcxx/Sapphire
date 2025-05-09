#pragma once

#include "SDK/api/src-deps/Core/Math/Color.h"
#include "SDK/api/src-deps/Core/Utility/PrimitiveTypes.h"
#include "src/SDK/api/src/common/world/phys/Vec3.h"

class Tessellator {
public:
    SDK_API void begin(const mce::PrimitiveMode mode, const int numVertices, const bool buildFaceData = false);

    SDK_API void vertex(float x, float y, float z);

    void vertex(const Vec3 &pos) {
        this->vertex(pos.x, pos.y, pos.z);
    }

    void vertexUV(const Vec3 &pos, const Vec2 &);

    void vertexUV(const Vec3 &pos, float, float);

    void vertexUV(float x, float y, float z, float, float);

    SDK_API void color(float r, float g, float b, float a);

    void color(int r, int g, int b, int a) {
        return this->color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }

    void color(const mce::Color &color) {
        return this->color(color.r, color.g, color.b, color.a);
    }

    void color(int color) {
        return this->color(color >> 24, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
    }
};