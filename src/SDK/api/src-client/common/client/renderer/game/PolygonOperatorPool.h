#pragma once

#include "pch.h" // IWYU pragma: keep

// size: 136
template <typename T>
class PolygonOperatorPool {
public:
    using Factory = std::function<std::unique_ptr<T>()>;

    // size: 16
    struct PoolEntry {
        const bool         mIsImmediate; // off+0
        std::unique_ptr<T> mEntry;       // off+8

        PoolEntry(bool isImmediate, std::unique_ptr<T> &&entry) :
            mIsImmediate(isImmediate), mEntry(std::move(entry)) {}

        PoolEntry(PoolEntry &&other) noexcept
            : mIsImmediate(other.mIsImmediate),
              mEntry(std::move(other.mEntry)) {}

        PoolEntry(const PoolEntry &) = delete;

        ~PoolEntry() noexcept = default;

        T &operator*() { return *mEntry; }
    };

    mutable std::mutex mMutex; // off+0

    const uint64_t mImmediateSize; // off+80
    const uint64_t mDeferredSize;  // off+88

    std::atomic<size_t> mUsedImmediate; // off+96
    std::atomic<size_t> mUsedDeferred;  // off+104

    std::vector<std::unique_ptr<T>> mPool; // off+112

    PolygonOperatorPool(const size_t immediateSize, const size_t deferredSize) :
        mImmediateSize(immediateSize), mDeferredSize(deferredSize), mUsedImmediate(0), mUsedDeferred(0) {
        mPool.reserve(immediateSize + deferredSize);
    }

    void reserve(const size_t count, const Factory &factory) {
        std::lock_guard<std::mutex> lock(mMutex);
        mPool.reserve(count);
        while (mPool.size() < count) {
            mPool.push_back(factory());
        }
    }

    std::optional<PoolEntry> tryGet(const bool isImmediate, const Factory &factory) {
        const size_t         maximum = isImmediate ? static_cast<size_t>(-1) : mDeferredSize;
        std::atomic<size_t> &counter = isImmediate ? mUsedImmediate : mUsedDeferred;

        std::unique_ptr<T> entry = _tryGet(counter, maximum, factory);
        if (entry) {
            return PoolEntry{isImmediate, std::move(entry)};
        }
        return std::nullopt;
    }

    void release(PoolEntry &&entry) {
        if (entry.mEntry) {
            {
                std::lock_guard<std::mutex> lock(mMutex);
                mPool.push_back(std::move(entry.mEntry));
            }
            if (entry.mIsImmediate) {
                --mUsedImmediate;
            } else {
                --mUsedDeferred;
            }
        }
    }

    void trim() {
        std::lock_guard<std::mutex> lock(mMutex);

        const size_t targetSize = mDeferredSize + mImmediateSize;
        while (mPool.size() > targetSize) {
            mPool.pop_back();
        }
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mMutex);
        mPool.clear();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mMutex);
        return mPool.size();
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mMutex);
        return mPool.empty();
    }

    size_t availableDeferred() const {
        const size_t used = mUsedDeferred.load(std::memory_order_relaxed);
        return (mDeferredSize > used) ? (mDeferredSize - used) : 0;
    }

    bool anyUsed() const {
        return mUsedImmediate.load(std::memory_order_relaxed) > 0 || mUsedDeferred.load(std::memory_order_relaxed) > 0;
    }

    std::unique_ptr<T> _tryGet(std::atomic<size_t> &counter, const size_t maximum, const Factory &factory) {
        if (++counter <= maximum) {
            std::lock_guard<std::mutex> lock(mMutex);

            if (mPool.empty()) {
                return factory();
            }

            std::unique_ptr<T> entry = std::move(mPool.back());
            mPool.pop_back();
            return entry;
        }

        --counter;
        return nullptr;
    }

    ~PolygonOperatorPool() noexcept = default;
};
