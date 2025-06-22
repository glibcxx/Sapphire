#pragma once

#include <vector>
#include "../Endstone/Guarded.hpp"

namespace dragon::memory {

    template <typename Handle, typename T>
    struct Block {
        Handle   mBuffer;
        uint32_t mOffset;
        uint32_t mSize;
    };

    template <typename Handle, typename T>
    class PartitionedFixedAllocator {
    public:
        using Block = Block<typename Handle, T>;
    };

    // size: 32
    template <typename Alloc, typename T>
    class PartitionedAllocator {
    public:
        using FixedAllocator = PartitionedFixedAllocator<typename Alloc::Handle, T>;
        using Block = typename FixedAllocator::Block;

        const uint32_t                               mSlabSize; // off+0
        const uint32_t                               mSlotSize; // off+4
        std::vector<std::unique_ptr<FixedAllocator>> mSlabs;    // off+8
    };

    // size: 120
    template <typename Alloc, typename T>
    class SynchronizedPartitionedAllocator {
    public:
        using Allocator = PartitionedAllocator<Alloc, T>;
        using Block = typename Allocator::Block;

        const uint32_t                      mSlotSize;      // off+0
        const bool                          mIsPassThrough; // off+4
        dragon::guarded::Guarded<Allocator> mAllocator;     // off+8
    };
} // namespace dragon::memory