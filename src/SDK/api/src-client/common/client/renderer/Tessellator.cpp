#include "Tessellator.h"

void Tessellator::begin(const mce::PrimitiveMode mode, const int maxVertices, const bool buildFaceData) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x83\xC3\x00\x00\x8D\x00\xE0\x04"_sig,
        &Tessellator::begin>;
    (this->*Hook::origin)(mode, maxVertices, buildFaceData);
}

void Tessellator::vertex(float x, float y, float z) {
    using Fn = void (Tessellator::*)(float, float, float);
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xF3\x0F\x10\x5B\x00\xF3\x0F\x10\x13"_sig,
        (Fn)&Tessellator::vertex>;
    (this->*Hook::origin)(x, y, z);
}

void Tessellator::vertexUV(float x, float y, float z, float u, float v) {
    using Fn = void (Tessellator::*)(float, float, float, float, float);
    using Hook = sapphire::ApiLoader<"\x48\x83\xEC\x00\x80\xB9\x00\x00\x00\x00\x00\x0F\x57\xE4"_sig, (Fn)&Tessellator::vertexUV>;
    (this->*Hook::origin)(x, y, z, u, v);
}

void Tessellator::color(float r, float g, float b, float a) {
    using Fn = void (Tessellator::*)(float, float, float, float);
    using Hook = sapphire::ApiLoader<"\x80\xB9\x00\x00\x00\x00\x00\x4C\x8B\xC1\x75"_sig, (Fn)&Tessellator::color>;
    (this->*Hook::origin)(r, g, b, a);
}

mce::Mesh Tessellator::end(UploadMode uploadMode, std::string_view debugName, bool a3) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x49\x8B\x45\x00\x48\xC7\x44\x24"_sig,
        &Tessellator::end>;
    return (this->*Hook::origin)(uploadMode, debugName, a3);
}
