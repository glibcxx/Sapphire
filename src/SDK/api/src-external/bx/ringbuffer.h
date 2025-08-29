#pragma once

namespace bx {

    // size: 16
    class RingBufferControl {
    public:
        const uint32_t m_size;
        uint32_t       m_current;
        uint32_t       m_write;
        uint32_t       m_read;
    };

} // namespace bx