#include "bgfx.h"
#include "SDK/core/ApiManager.h"

bgfx::Context *bgfx::s_ctx = sapphire::loadStatic<
    sapphire::deRefMov | "\x48\x8B\x15\x00\x00\x00\x00\x45\x33\xDB"_sig,
    &bgfx::s_ctx,
    bgfx::Context *>();

bx::AllocatorI *bgfx::g_allocator = sapphire::loadStatic<
    sapphire::deRefMov | "\x48\x8B\x0D\x00\x00\x00\x00\x81\xC7"_sig,
    &bgfx::g_allocator,
    bx::AllocatorI *>();

uint16_t bgfx::Encoder::setScissor(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height) {
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x48\x89\x7C\x24\x00\x4C\x8B\x11\x0F\xB7\xFA"_sig, // 1.21.50
        &Encoder::setScissor>;
    return (this->*Hook::origin)(_x, _y, _width, _height);
}

int bgfx::init(const bgfx::Init &_init, bool a2) {
    using Hook = sapphire::ApiLoader<
        "\x40\x53\x56\x57\x48\x83\xEC\x00\x0F\xB6\xF2"_sig, // 1.21.50
        &init>;
    return (Hook::origin)(_init, a2);
}

int bgfx::initCaps(const bgfx::Init &_init, int &a2) {
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x48\x8B\xFA\x48\x8B\xD9\x48\x83\x3D"_sig, // 1.21.50
        &initCaps>;
    return (Hook::origin)(_init, a2);
}

bgfx::DynamicVertexBufferHandle bgfx::createDynamicVertexBuffer(
    const bgfx::Memory     *_mem,
    const bgfx::VertexDecl &_decl,
    uint16_t                _flags
) {
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x41\x0F\xB7\xF8"_sig, // 1.21.50
        &createDynamicVertexBuffer>;
    return (Hook::origin)(_mem, _decl, _flags);
}

bgfx::DynamicIndexBufferHandle bgfx::createDynamicIndexBuffer(const bgfx::Memory *_mem, uint16_t _flags) {
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x56\x41\x57\x48\x83\xEC\x00\x0F\xB7\xFA"_sig, // 1.21.50
        createDynamicIndexBuffer>;
    return (Hook::origin)(_mem, _flags);
}