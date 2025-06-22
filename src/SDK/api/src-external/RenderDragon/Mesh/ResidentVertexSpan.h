#pragma once

#include <optional>
#include "SDK/api/src-external/RenderDragon/Memory/PartitionedAllocator.h"
#include "SDK/api/src-external/RenderDragon/Rendering/BufferHandleType.h"
#include "VertexFormat.h"

namespace dragon::mesh {

    namespace details {
        struct VertexBufferHandleTypeKey;
    }

    // size: 1
    class VertexBufferAllocator {
    public:
        using Handle = std::optional<dragon::rendering::details::BufferHandleTypeWrapper<details::VertexBufferHandleTypeKey>>;
    };

    // size: 312 (1.21.50)
    class VertexSpanContext {
    public:
        VertexBufferResourceManager                                                      &mVbResourceManager;    // off+0
        dragon::memory::SynchronizedPartitionedAllocator<VertexBufferAllocator, uint32_t> mPartitionedAllocator; // off+8
        VertexFormat                                                                      mBytesDecl;            // off+128
    };

    struct VertexSpanType {
        using HandleTypeKey = details::VertexBufferHandleTypeKey;
        using Context = VertexSpanContext;
        using KeyType = uint16_t;
    };

} // namespace dragon::mesh