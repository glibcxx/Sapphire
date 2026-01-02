#pragma once

namespace Bedrock {

    template <typename T, std::size_t CacheSize = 64, typename Alloc = std::allocator<T>, int T3 = 0>
    class MPSCQueue {
    public:
        struct Node {
            Node            *next; // off+0
            std::optional<T> data; // off+8
        };

        __int64              mUnk0;
        Node                *mNodeCache[CacheSize];
        uint64_t             cacheCursor;
        std::atomic<int64_t> activeNodeCount;
        std::atomic<int64_t> cachedNodeCount;
        std::atomic<Node *>  tail;
        Node                *head;
    };

} // namespace Bedrock