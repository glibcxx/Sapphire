#pragma once

#include "pch.h" // IWYU pragma: keep

#include "SDK/api/src-external/RenderDragon/Memory/PartitionedAllocator.h"
#include "SDK/api/src-external/RenderDragon/Rendering/BufferHandleType.h"

namespace dragon::mesh {

    class IndexBufferResourceManager;

    namespace details {
        struct IndexBufferHandleTypeKey;
    }

    // size: 1
    class IndexBufferAllocator {
    public:
        using Handle = std::optional<dragon::rendering::details::BufferHandleTypeWrapper<details::IndexBufferHandleTypeKey>>;
    };

    // size: 128
    class IndexSpanContext {
    public:
        IndexBufferResourceManager                                                      &mIbResourceManager;    // off+0
        dragon::memory::SynchronizedPartitionedAllocator<IndexBufferAllocator, uint32_t> mPartitionedAllocator; // off+16
    };

    struct IndexSpanType {
        using HandleTypeKey = details::IndexBufferHandleTypeKey;
        using Context = IndexSpanContext;
        using KeyType = uint16_t;
    };

} // namespace dragon::mesh