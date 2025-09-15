#include "renderer_d3d12_internal.h"
#include "SDK/core/ApiManager.h"
#include "util/Memory.hpp"

void bgfx::d3d12::CommandContextD3D12::setBlendState(
    D3D12_BLEND_DESC &_desc,
    uint64_t          _state,
    uint32_t          _rgba
) {
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x4C\x8D\x1D"_sig, // 1.21.50
        &CommandContextD3D12::setBlendState>;
    (this->*Hook::origin)(_desc, _state, _rgba);
}

uint32_t bgfx::d3d12::CommandContextD3D12::setInputLayout(
    D3D12_INPUT_ELEMENT_DESC        *_vertexElements,
    uint8_t                          _numStreams,
    const bgfx::VertexDecl         **_vertexDecls,
    const bgfx::d3d12::ProgramD3D12 &_program,
    uint16_t                         _numInstanceData
) {
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x48\x8B\x45\x00\x49\x8B\xF1"_sig, // 1.21.50
        &CommandContextD3D12::setInputLayout>;
    return (this->*Hook::origin)(_vertexElements, _numStreams, _vertexDecls, _program, _numInstanceData);
}

ID3D12PipelineState *bgfx::d3d12::CommandContextD3D12::getPipelineState(
    bgfx::d3d12::RendererContextD3D12 &_context,
    uint64_t                           _state,
    uint64_t                           _stencil,
    int32_t                            _biasConst,
    float                              _biasSlope,
    float                              _biasClamp,
    uint8_t                            _numStreams,
    const bgfx::VertexDecl           **_vertexDecls,
    uint16_t                           _programIdx,
    uint8_t                            _numInstanceData,
    bgfx::FrameBufferHandle            _fbh,
    const DXGI_SAMPLE_DESC            &_sampleDesc
) {
    using Hook = sapphire::ApiLoader<
        [](uintptr_t addr) { return memory::deRef(addr + 5, memory::AsmOperation::CALL); } | "\x44\x89\x6C\x24\x00\xE8\x00\x00\x00\x00\x4C\x8B\xC0"_sig, // 1.21.50
        &CommandContextD3D12::getPipelineState>;
    return (this->*Hook::origin)(_context, _state, _stencil, _biasConst, _biasSlope, _biasClamp, _numStreams, _vertexDecls, _programIdx, _numInstanceData, _fbh, _sampleDesc);
}

void bgfx::d3d12::RendererContextD3D12::postFlip() {
    using Hook = sapphire::ApiLoader<
        "\x40\x53\x48\x83\xEC\x00\x48\x83\xB9\x00\x00\x00\x00\x00\x48\x8B\xD9\x0F\x84"_sig, // 1.21.50
        &RendererContextD3D12::postFlip,
        SPHR_FUNCDNAME>;
    (this->*Hook::origin)();
}

bool bgfx::d3d12::RendererContextD3D12::init(const bgfx::Init &_init) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x44\x0F\xB6\xF0\x84\xC0\x74"_sig, // 1.21.50
        &RendererContextD3D12::init,
        SPHR_FUNCDNAME>;
    return (this->*Hook::origin)(_init);
}

bgfx::d3d12::RendererContextD3D12 *bgfx::d3d12::s_renderD3D12 = sapphire::loadStatic<
    sapphire::deRefMov | "\x48\x8B\x05\x00\x00\x00\x00\x48\x8B\xF9\x33\xD2"_sig,
    &bgfx::d3d12::s_renderD3D12,
    bgfx::d3d12::RendererContextD3D12 *>();