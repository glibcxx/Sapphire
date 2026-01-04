#pragma once

// size: 136
template <typename T>
class PolygonOperatorPool {
public:
    using Factory = std::function<std::unique_ptr<T>()>;

    // size: 16
    struct PoolEntry {
        const bool         mIsImmediate; // off+0
        std::unique_ptr<T> mEntry;       // off+8
    };

    std::mutex mMutex; // off+0

    const uint64_t mImmediateSize; // off+80
    const uint64_t mDeferredSize;  // off+88

    std::atomic<size_t> mUsedImmediate; // off+96
    std::atomic<size_t> mUsedDeferred;  // off+104

    std::vector<std::unique_ptr<T>> mPool; // off+112
};
