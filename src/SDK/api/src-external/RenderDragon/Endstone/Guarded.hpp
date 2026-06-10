#pragma once

#include "pch.h" // IWYU pragma: keep

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