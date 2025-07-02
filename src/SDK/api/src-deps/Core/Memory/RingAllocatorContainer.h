#pragma once

#include "CpuRingBufferAllocator.h"

namespace Core {

    template <typename TBuffer, size_t M = 2, size_t N = 0, typename TRing = Core::CheckedRingBuffer<M, N>>
    class RingAllocatorContainer {
    public:
        using Ring = FencedDynamicRingBuffer<Core::CpuRingBufferAllocation_Buffer, M, N>;
        using Allocator = Core::CpuRingBufferAllocator<TBuffer, M, N, Core::CheckedRingBuffer<M, N>>;

        struct AllocationScope {
        };

        typename Ring::Ring::Fence              mCurrentFence;          // off+0
        std::atomic<typename Ring::Ring::Fence> mLastCompletedFence;    // off+8
        std::atomic<bool>                       mHasLastCompletedFence; // off+16
        typename Ring::RingPtr                  mBuffer;                // off+24
        Ring                                    mRing;                  // off+40
    };

} // namespace Core