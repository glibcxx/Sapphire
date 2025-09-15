#pragma once

namespace bx {
	// size: 64
    class Mutex {
    public:
        alignas(16) uint8_t m_internal[64];
    };

	// size: 8
    class MutexScope {
    public:
        Mutex &m_mutex;
    };

} // namespace bx
