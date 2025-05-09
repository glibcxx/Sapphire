#include "Tessellator.h"

void Tessellator::begin(const mce::PrimitiveMode mode, const int maxVertices, const bool buildFaceData) {
    using Hook = core::ApiLoader<
        "\xE8\x00\x00\x00\x00\x48\x83\xC3\x00\x00\x8D\x00\xE0\x04"_sig,
        &Tessellator::begin,
        [](auto addr) {
            return memory::deRef(addr, memory::AsmOperation::CALL);
        }>;
    (this->*Hook::origin)(mode, maxVertices, buildFaceData);
}

void Tessellator::vertex(float x, float y, float z) {
    using Fn = void (Tessellator::*)(float, float, float);
    using Hook = core::ApiLoader<
        "\xE8\x00\x00\x00\x00\xF3\x0F\x10\x5B\x00\xF3\x0F\x10\x13"_sig,
        (Fn)&Tessellator::vertex,
        [](auto addr) {
            return memory::deRef(addr, memory::AsmOperation::CALL);
        }>;
    (this->*Hook::origin)(x, y, z);
}

void Tessellator::color(float r, float g, float b, float a) {
    using Fn = void (Tessellator::*)(float, float, float, float);
    using Hook = core::ApiLoader<"\x80\xB9\x00\x00\x00\x00\x00\x4C\x8B\xC1\x75"_sig, (Fn)&Tessellator::color>;
    (this->*Hook::origin)(r, g, b, a);
}