#pragma once

#include <mutex>
#include <shared_mutex>

namespace Bedrock {

namespace Threading {

    typedef std::mutex              Mutex;
    typedef std::shared_timed_mutex SharedMutex;
    typedef std::recursive_mutex    RecursiveMutex;

    template <typename T>
    using UniqueLock = std::unique_lock<T>;

    template <typename T>
    using LockGuard = std::lock_guard<T>;

    template <typename T>
    using SharedLock = std::shared_lock<T>;

};

}; // namespace Bedrock::Threading
