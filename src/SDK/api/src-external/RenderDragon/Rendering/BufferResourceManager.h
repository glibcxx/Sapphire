#pragma once

#include <atomic>
#include "BufferHandleType.h"
#include "SDK/api/src-deps/Core/Threading/MPMCQueue.h"
#include "SDK/api/src-deps/Core/Container/Cache.h"

namespace dragon::rendering {

    class GraphicsTasks;

    enum class BufferState : int {
        Pending = 0,
        CreateQueued = 1,
        Created = 2,
        MarkedForDeletion = 3,
    };

    // size: 704
    template <typename Buffer>
    class BufferResourceManager {
    public:
        // size: 736 (1.21.50, with Buffer=dragon::mesh::VertexBufferType)
        struct BufferResource {
            std::mutex mAccessLock; // off+0
            std::vector<std::variant<
                typename Buffer::ItemResourceUpdateParameters,
                typename Buffer::OffsetResourceUpdateParameters>>
                mDeferredOrderedUpdateSequence; // off+80

            BufferState                                                mState;              // off+104
            bool                                                       mWasEverCreated;     // off+108
            bool                                                       mWasEverUsed;        // off+109
            Buffer::ResourceType                                       mResource;           // off+110
            Buffer::ResourceDescription                                mDescription;        // off+120
            Buffer::ResourcePublicDescription                          mPublicDescription;  // off+144
            std::optional<typename Buffer::ResourceCreationParameters> mCreationParameters; // off+368
        };

        GraphicsTasks        &mGraphicsTasks; // off+0
        std::atomic<uint64_t> mHandleCounter; // off+8
        MPMCQueue<std::optional<details::BufferHandleTypeWrapper<typename Buffer::HandleTypeKey>>>
            mBuffersDeletionQueue; // off+16

        Core::Cache<uint64_t, BufferResource, std::shared_ptr<BufferResource>> mBuffers; // off+632
    };

} // namespace dragon::rendering