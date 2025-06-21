#pragma once

#include <atomic>
#include "BufferHandleType.h"
#include "SDK/api/src-deps/Core/Threading/MPMCQueue.h"
#include "SDK/api/src-deps/Core/Container/Cache.h"

namespace dragon::rendering {

    class GraphicsTasks;

    // size: 704
    template <typename Buffer>
    class BufferResourceManager {
    public:
        struct BufferResource {};

        GraphicsTasks                                                                             &mGraphicsTasks;        // off+0
        std::atomic<uint64_t>                                                                      mHandleCounter;        // off+8
        MPMCQueue<std::optional<details::BufferHandleTypeWrapper<typename Buffer::HandleTypeKey>>> mBuffersDeletionQueue; // off+16
        Core::Cache<uint64_t, BufferResource, std::shared_ptr<BufferResource>>                     mBuffers;              // off+632
    };


} // namespace dragon::rendering