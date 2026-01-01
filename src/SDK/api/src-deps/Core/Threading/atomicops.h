#pragma once

namespace Lockless {

    enum class MemoryOrder : int {
        Relaxed = 0,
        Acquire = 1,
        Release = 2,
        AcqRel = 3,
        SeqCst = 4,
    };

    template <typename T>
    class WeakAtomic {
    public:
        std::atomic<T> mValue; // off+0
    };

} // namespace Lockless