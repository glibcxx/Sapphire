#include "IndexBufferContainer.h"

void mce::IndexBufferContainer::loadIndexBuffer(
    mce::BufferResourceService &bufferResourceService,
    const uint32_t              stride,
    const char *const           data,
    const uint32_t              count,
    std::string_view            debugName
) {
    // search "loadIndexBuffer"
    // \x48\x8B\xC4\x4C\x89\x48\x00\x44\x89\x40 1.21.50
}