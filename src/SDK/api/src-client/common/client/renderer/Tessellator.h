#pragma once

#include "SDK/api/src-deps/Core/Math/Color.h"
#include "SDK/api/src-deps/Core/Utility/PrimitiveTypes.h"
#include "src/SDK/api/src/common/world/phys/Vec3.h"

class Tessellator {
public:
    SDK_API void begin(mce::PrimitiveMode mode, int numVertices);

    SDK_API void vertex(float x, float y, float z);

    void vertex(const Vec3 &pos);

    void vertexUV(const Vec3 &pos, const Vec2 &);

    void vertexUV(const Vec3 &pos, float, float);

    void vertexUV(float x, float y, float z, float, float);

    void color(float r, float g, float b, float a);

    void color(int r, int g, int b, int a);

    void color(const mce::Color &color);

    void color(int color);
};