#include "renderer_d3d12_internal.h"
#include "SDK/core/SymbolResolver.h"

bgfx::d3d12::RendererContextD3D12 *bgfx::d3d12::s_renderD3D12 = sapphire::bind::data<
    sapphire::deRefMov | "\x48\x8B\x05\x00\x00\x00\x00\x48\x8B\xF9\x33\xD2"_sig,
    &bgfx::d3d12::s_renderD3D12,
    bgfx::d3d12::RendererContextD3D12 *>();