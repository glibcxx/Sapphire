#include "ModelPart.h"
#include "SDK/core/SymbolResolver.h"

ModelPart *ModelPart::ctor(
    int xTexOffs,
    int yTexOffs,
    int texWidth,
    int texHeight
) {
    using Bind = sapphire::bind::Fn<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x4C\x8D\x00\x00\x00\x00\x00\xC7\x44\x24\x20\x20\x00\x00\x00\x41\xB9\x40\x00\x00\x00\x45\x33\xC0\x33\xD2\x49\x8B"_sig,
        &ModelPart::ctor>;
    return (this->*Bind::origin)(xTexOffs, yTexOffs, texWidth, texHeight);
}

void ModelPart::dtor() noexcept {
    using Bind = sapphire::bind::Fn<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xF3\x0F\x11\x7C\x24\x00\xC7\x44\x24"_sig,
        &ModelPart::dtor>;
    return (this->*Bind::origin)();
}