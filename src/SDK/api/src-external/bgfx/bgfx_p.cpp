#include "bgfx_p.h"
#include "SDK/core/ApiManager.h"

void bgfx::Frame::create() {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x40\x38\x35"_sig, // 1.21.50
        &Frame::create>;
    (this->*Hook::origin)();
}

void bgfx::Frame::destroy() {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xC7\x86\x00\x00\x00\x00\x00\x00\x00\x00\x32\xC0"_sig, // 1.21.50
        &Frame::destroy>;
    (this->*Hook::origin)();
}

int32_t bgfx::Context::renderThread(bx::Thread *a1, void *a2) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefLea | "\x4C\x8D\x05\x00\x00\x00\x00\x48\x89\x7B"_sig, // 1.21.50
        &Context::renderThread>;
    return (Hook::origin)(a1, a2);
}

int bgfx::Context::init(bool a2) {
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x0F\xB6\xFA\xBE"_sig, // 1.21.50
        &Context::init>;
    return (this->*Hook::origin)(a2);
}

void bgfx::Context::initRenderer() {
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x81\xA1"_sig, // 1.21.50
        &Context::initRenderer>;
    (this->*Hook::origin)();
}

bgfx::RenderFrame::Enum bgfx::Context::renderFrame(int32_t _msecs) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x83\xF8\x00\x75\x00\x48\x89\x7C\x24"_sig, // 1.21.50
        &Context::renderFrame>;
    return (this->*Hook::origin)(_msecs);
}

bool bgfx::Context::apiSemWait(int32_t _msecs) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x80\xBF\x00\x00\x00\x00\x00\x48\xC7\x05"_sig, // 1.21.50
        &Context::apiSemWait>;
    return (this->*Hook::origin)(_msecs);
}

void bgfx::Context::resetView(uint16_t _id) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x41\xFF\xC1\x41\x81\xF9"_sig, // 1.21.50
        &Context::resetView>;
    return (this->*Hook::origin)(_id);
}

void bgfx::Context::frameNoRenderWait(bool a2) {
    using Hook = sapphire::ApiLoader<
        "\x40\x53\x48\x83\xEC\x00\x4C\x8B\x81\x00\x00\x00\x00\x48\x8B\xD9\x49\x81\xC0"_sig, // 1.21.50
        &Context::frameNoRenderWait>;
    return (this->*Hook::origin)(a2);
}

void bgfx::Context::swap() {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x41\x80\xBE\x00\x00\x00\x00\x00\x75\x00\x45\x33\xC0"_sig, // 1.21.50
        &Context::swap>;
    return (this->*Hook::origin)();
}

void bgfx::Context::freeDynamicBuffers() {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\xBF"_sig, // 1.21.50
        &Context::freeDynamicBuffers>;
    return (this->*Hook::origin)();
}

void bgfx::Context::freeAllHandles(bgfx::Frame *_frame) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x4C\x8B\x93\x00\x00\x00\x00\x4C\x8B\xCE"_sig, // 1.21.50
        &Context::freeAllHandles>;
    return (this->*Hook::origin)(_frame);
}

bgfx::IndexBufferHandle bgfx::Context::createIndexBuffer(const bgfx::Memory *_mem, uint16_t _flags) {
    using Hook = sapphire::ApiLoader<
        "\x40\x53\x55\x56\x57\x41\x56\x48\x83\xEC\x00\x45\x0F\xB7\xF1"_sig, // 1.21.50
        &Context::createIndexBuffer>;
    return (this->*Hook::origin)(_mem, _flags);
}

void bgfx::Context::destroyIndexBuffer(bgfx::IndexBufferHandle _handle) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\xCB\xE8\x00\x00\x00\x00\x48\x85\xC0\x75\x00\x0F\xB7\x95\x00\x00\x00\x00\xBF\x00\x00\x00\x00\x48\x83\xC2\x00\x49\x83\xC4\x00\x48\x8D\x04\x32"_sig, // 1.21.50
        &Context::destroyIndexBuffer>;
    (this->*Hook::origin)(_handle);
}

bgfx::VertexBufferHandle bgfx::Context::createVertexBuffer(
    const bgfx::Memory     *_mem,
    const bgfx::VertexDecl &_decl,
    uint16_t                _flags
) {
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x4D\x8B\xE1\x49\x8B\xF8"_sig, // 1.21.50
        &Context::createVertexBuffer>;
    return (this->*Hook::origin)(_mem, _decl, _flags);
}

bgfx::DynamicVertexBufferHandle bgfx::Context::createDynamicVertexBuffer(
    uint32_t                _num,
    const bgfx::VertexDecl &_decl,
    uint16_t                _flags
) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x0F\xB7\x08\x66\x89\x0B\xC6\x43\x00\x00\xEB\x00\x80\xBA"_sig, // 1.21.50
        &Context::createDynamicVertexBuffer>;
    return (this->*Hook::origin)(_num, _decl, _flags);
}

bgfx::DynamicIndexBufferHandle bgfx::Context::createDynamicIndexBuffer(
    uint32_t _num,
    uint16_t _flags
) {
    using Hook = sapphire::ApiLoader<
        "\x40\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x45\x0F\xB7\xF9"_sig, // 1.21.50
        &Context::createDynamicIndexBuffer>;
    return (this->*Hook::origin)(_num, _flags);
}

void bgfx::Context::update(
    bgfx::DynamicVertexBufferHandle _handle,
    uint32_t                        _startVertex,
    const bgfx::Memory             *_mem
) {
    using Fn = void (bgfx::Context::*)(bgfx::DynamicVertexBufferHandle, uint32_t, const bgfx::Memory *);
    using Hook = sapphire::ApiLoader<
        "\x40\x53\x55\x56\x57\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x00\x4D\x8B\xF9\x45\x8B\xE0"_sig, // 1.21.50
        (Fn)&Context::update>;
    return (this->*Hook::origin)(_handle, _startVertex, _mem);
}

void bgfx::Context::update(
    bgfx::DynamicIndexBufferHandle _handle,
    uint32_t                       _startIndex,
    const bgfx::Memory            *_mem
) {
    using Fn = void (bgfx::Context::*)(bgfx::DynamicIndexBufferHandle, uint32_t, const bgfx::Memory *);
    using Hook = sapphire::ApiLoader<
        "\x40\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x4D\x8B\xE9\x45\x8B\xE0"_sig, // 1.21.50
        (Fn)&Context::update>;
    return (this->*Hook::origin)(_handle, _startIndex, _mem);
}

uint32_t bgfx::Context::frame(uint32_t _flags) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x87\x83\x00\x00\x00\x00\x48\x8B\x4B\x00\xC6\x43\x00\x00\xE8"_sig, // 1.21.50
        &Context::frame>;
    return (this->*Hook::origin)(_flags);
}

void bgfx::EncoderImpl::setUniform(
    bgfx::UniformType::Enum _type,
    bgfx::UniformHandle     _handle,
    const void             *_value,
    uint16_t                _num
) {
    using Hook = sapphire::ApiLoader<
        "\x40\x53\x55\x56\x57\x41\x56\x41\x57\x48\x83\xEC\x00\x48\x8B\x01\x4D\x8B\xF9"_sig, // 1.21.50
        &EncoderImpl::setUniform>;
    (this->*Hook::origin)(_type, _handle, _value, _num);
}

void bgfx::EncoderImpl::discard() {
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x6C\x24\x00\x48\x89\x7C\x24\x00\x41\x56\x48\x83\xEC\x00\x48\x8B\xF9"_sig, // 1.21.50
        &EncoderImpl::discard>;
    (this->*Hook::origin)();
}

void bgfx::EncoderImpl::submit(
    uint16_t                   _id,
    bgfx::ProgramHandle        _program,
    bgfx::OcclusionQueryHandle _occlusionQuery,
    int32_t                    _depth,
    bool                       _preserveState
) {
    using Fn = void (bgfx::EncoderImpl::*)(uint16_t, bgfx::ProgramHandle, bgfx::OcclusionQueryHandle, int32_t, bool);
    using Hook = sapphire::ApiLoader<
        "\x40\x53\x48\x83\xEC\x00\x80\xB9\x00\x00\x00\x00\x00\x48\x8B\xD9\x44\x0F\xB7\xD2"_sig, // 1.21.50
        (Fn)&EncoderImpl::submit>;
    (this->*Hook::origin)(_id, _program, _occlusionQuery, _depth, _preserveState);
}

decltype(bgfx::sphr_none_export::fpFatal) bgfx::sphr_none_export::fpFatal = sapphire::ApiLoader<
    "\x48\x89\x54\x24\x00\x4C\x89\x44\x24\x00\x4C\x89\x4C\x24\x00\x55\x53\x56\x57\x41\x54\x41\x55"_sig, // 1.21.50
    (decltype(bgfx::sphr_none_export::fpFatal))&bgfx::fatal>::origin;

SPHR_NOINLINE void bgfx::fatal(bgfx::Fatal::Enum _code, const char *_format, ...) {
    (sphr_none_export::fpFatal)(_code, "");
}
