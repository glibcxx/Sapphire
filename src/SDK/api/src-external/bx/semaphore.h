#pragma once

namespace bx {

    // size: 128
    class Semaphore {
    public:
        alignas(16) uint8_t m_internal[128];
    };

} // namespace bx
