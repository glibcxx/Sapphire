#pragma once

#include "SDK/api/src/common/world/phys/Vec3.h"
#include "SDK/api/src-deps/Core/Math/Color.h"
#include "SDK/api/src-deps/Core/Utility/PrimitiveTypes.h"
#include "SDK/api/src-deps/MinecraftRenderer/Renderer/Mesh.h"

class Tessellator {
public:
    enum class UploadMode : int32_t {
        Buffered = 0,
        Manual = 1,
        Never = 2,
    };

    SDK_API void begin(const mce::PrimitiveMode mode, const int numVertices, const bool buildFaceData = false);
    MARK_HOOKABLE(&Tessellator::begin)

    SDK_API void vertex(float x, float y, float z);
    MARK_HOOKABLE(&Tessellator::vertex)

    SDK_API void vertexUV(float x, float y, float z, float u, float v);
    MARK_HOOKABLE(&Tessellator::vertexUV)

    SDK_API void color(float r, float g, float b, float a);
    MARK_HOOKABLE(&Tessellator::color)

    SDK_API mce::Mesh end(UploadMode uploadMode, std::string_view debugName, bool a3);
    MARK_HOOKABLE(&Tessellator::end)

    void vertex(const Vec3 &pos) {
        this->vertex(pos.x, pos.y, pos.z);
    }

    void vertexUV(const Vec3 &pos, const Vec2 &uv) {
        this->vertexUV(pos.x, pos.y, pos.z, uv.x, uv.y);
    }

    void vertexUV(const Vec3 &pos, float u, float v) {
        this->vertexUV(pos.x, pos.y, pos.z, u, v);
    }

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