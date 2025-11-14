#include "IndexBufferContainer.h"

void mce::IndexBufferContainer::loadIndexBuffer(
    mce::BufferResourceService &bufferResourceService,
    const uint32_t              stride,
    const char *const           data,
    const uint32_t              count,
    std::string_view            debugName
) {
    // search "loadIndexBuffer"

    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x41\x8B\x00\x00\x89\x43\x20\x48\x8D"_sig,
        &IndexBufferContainer::loadIndexBuffer>;
    (this->*Hook::origin)(bufferResourceService, stride, data, count, debugName);
}