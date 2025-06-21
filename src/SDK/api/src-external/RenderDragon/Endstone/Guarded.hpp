#pragma once

#include <mutex>

namespace dragon::guarded {

    template <typename T, typename Mutex = std::mutex>
    class Guarded {
    public:
        T     mValue;
        Mutex mMutex;

        using ValueType = T;
        using MutexType = Mutex;
    };

} // namespace dragon::guarded