#pragma once

#include "FencedDynamicRingBuffer.h"
#include <variant>

namespace bgfx {

    template <typename TBuffer, typename TSize, typename TPtr, typename TAlloc>
    class RingBuffer {
    public:
        // size: 56
        struct Location {
            uint32_t m_offset;  // off+0
            uint32_t m_size;    // off+4
            TBuffer  m_buffer;  // off+8
            uint64_t m_gpuAddr; // off+40
            void    *m_cpuAddr; // off+48
        };

        // size: 1
        class RingBufferResourceAllocator {};

        RingBufferResourceAllocator                                      m_ringsAllocator; // off+0
        bgfx::FencedDynamicRingBuffer<RingBufferResourceAllocator, 2, 0> m_rings;          // off+8
    };

} // namespace bgfx