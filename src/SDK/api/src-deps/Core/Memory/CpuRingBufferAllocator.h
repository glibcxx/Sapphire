#pragma once

#include "CpuRingBuffer.h"
#include "FencedDynamicRingBuffer.h"

namespace Core {

    template <size_t M = 2, size_t N = 0>
    class CheckedRingBuffer {
    public:
        using Buffer = FencedDynamicRingBuffer<Core::CpuRingBufferAllocation_Buffer, M, N>;
    };

    template <typename TBuffer, size_t M = 2, size_t N = 0, typename TRing = Core::CheckedRingBuffer<M, N>>
    class CpuRingBufferAllocator {
    public:
        TRing *mBufferInstance;
    };

} // namespace Core