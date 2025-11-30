#include "bgfx.h"
#include "SDK/core/SymbolResolver.h"

bgfx::Context *bgfx::s_ctx = sapphire::bind::data<
    sapphire::deRefMov | "\x48\x8B\x15\x00\x00\x00\x00\x45\x33\xDB"_sig,
    &bgfx::s_ctx,
    bgfx::Context *>();

bx::AllocatorI *bgfx::g_allocator = sapphire::bind::data<
    sapphire::deRefMov | "\x48\x8B\x0D\x00\x00\x00\x00\x81\xC7"_sig,
    &bgfx::g_allocator,
    bx::AllocatorI *>();
