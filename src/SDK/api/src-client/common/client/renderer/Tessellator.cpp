#include "Tessellator.h"

void Tessellator::begin(mce::PrimitiveMode mode, int numVertices) {
    using Hook = core::ApiLoader<
        "\x48\x89\x5C\x24\x00\x56\x48\x83\xEC\x00\x80\xB9\x00\x00\x00\x00\x00\x45\x0F\xB6\xD1"_sig,
        &Tessellator::begin>;
    (this->*Hook::origin)(mode, numVertices);
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