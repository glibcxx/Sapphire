#pragma once

#include "CpuRingBuffer.h"
#include "FencedDynamicRingBuffer.h"

namespace Core {

    template <typename TBuffer, size_t M, size_t N, typename TRing>
    class CpuRingBufferAllocator {
    public:
    };

    template <size_t M = 2, size_t N = 0>
    class CheckedRingBuffer {
    public:
        using Buffer = FencedDynamicRingBuffer<Core::CpuRingBufferAllocation_Buffer, M, N>;
    };

} // namespace Core