#pragma once

#include <map>
#include <atomic>
#include <memory>
#include "FencedRingBuffer.h"

namespace bgfx {

    template <typename TAlloc, size_t M, size_t N>
    class FencedDynamicRingBuffer {
    public:
        using BufferSize = size_t;
        using BufferOffset = size_t;
        using Fence = size_t;
        using OptionalFence = std::optional<Fence>;

        using RingMap = std::map<BufferOffset, std::shared_ptr<FencedRingBuffer<FencedDynamicRingBuffer, N>>>;

        std::atomic<FencedRingBuffer<FencedDynamicRingBuffer, N> *>                           m_lastPageAllocatedFrom;        // off+0
        std::atomic<size_t>                                                                   m_allocationScopeCheck;         // off+8
        const BufferSize                                                                      m_initialSize;                  // off+16
        std::atomic<BufferSize>                                                               m_usedSize;                     // off+24
        std::atomic<BufferSize>                                                               m_allocationSize;               // off+32
        std::map<BufferOffset, std::shared_ptr<FencedRingBuffer<FencedDynamicRingBuffer, N>>> m_rings;                        // off+40
        uint16_t                                                                              m_ringLifetimeWithNoAllocation; // off+56
        Fence                                                                                 m_currentFence;                 // off+64
        OptionalFence                                                                         m_lastCompletedFenceValue;      // off+72
        TAlloc                                                                               &m_bufferImpl;                   // off+88
        std::shared_mutex                                                                     m_accessLock;                   // off+96
    };

} // namespace bgfx