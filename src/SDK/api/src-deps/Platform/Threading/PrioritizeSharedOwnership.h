#pragma once

#include <shared_mutex>
#include <condition_variable>
#include <atomic>

namespace Bedrock {

namespace Threading {

    class PrioritizeSharedOwnership {
    public:
        std::shared_mutex           mMutex;       // off+0
        std::condition_variable_any mZeroReaders; // off+8
        std::atomic<size_t>         mReaderCount; // off+96

        void lock();

        void lock_shared();

        bool try_lock();

        bool try_lock_shared();

        void unlock();

        void unlock_shared() {
            const size_t WRITER_WAITING_FLAG = 1ULL << 63;
            const size_t MASK_TO_CLEAR_WRITER_FLAG = ~WRITER_WAITING_FLAG;
            if (mReaderCount.fetch_sub(1, std::memory_order_release) == (WRITER_WAITING_FLAG | 1)) {
                mMutex.lock();
                mReaderCount.fetch_and(MASK_TO_CLEAR_WRITER_FLAG, std::memory_order_release);
                mMutex.unlock();
                mZeroReaders.notify_all();
            }
        }
    };

}

} // namespace Bedrock::Threading