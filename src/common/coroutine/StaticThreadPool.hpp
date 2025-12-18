#pragma once

#include <atomic>
#include <coroutine>
#include <deque>
#include <vector>
#include <thread>
#include <memory>
#include <random>
#include <cassert>
#include <limits>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <concurrentqueue.h>

#include "Constant.hpp"

namespace sapphire::coro {

    class StaticThreadPool;

    namespace detail {

        class WorkStealingQueue {
        public:
            WorkStealingQueue(size_t capacity) :
                mBuffer(std::make_unique<std::coroutine_handle<>[]>(capacity)),
                mCapacity(capacity > 1 ? std::bit_ceil(capacity) : 2) {
            }

            WorkStealingQueue(WorkStealingQueue &&other) :
                mBuffer(std::move(other.mBuffer)),
                mCapacity(other.mCapacity) {}

            bool push(std::coroutine_handle<> handle) noexcept {
                size_t bottom = mBottom.load(std::memory_order_relaxed);
                size_t top = mTop.load(std::memory_order_acquire);
                if (bottom - top >= mCapacity) return false;
                mBuffer[bottom & (mCapacity - 1)] = handle;
                mBottom.store(bottom + 1, std::memory_order_release);
                return true;
            }

            std::coroutine_handle<> pop() noexcept {
                size_t bottom = mBottom.load(std::memory_order_relaxed);
                size_t top = mTop.load(std::memory_order_acquire);

                if (bottom <= top) return nullptr;

                bottom = bottom - 1;
                mBottom.store(bottom, std::memory_order_relaxed);
                std::atomic_thread_fence(std::memory_order_seq_cst);

                top = mTop.load(std::memory_order_relaxed);
                if (top > bottom) {
                    mBottom.store(bottom + 1, std::memory_order_relaxed);
                    return nullptr;
                }

                auto handle = mBuffer[bottom & (mCapacity - 1)];
                if (top == bottom) {
                    if (!mTop.compare_exchange_strong(
                            top, top + 1, std::memory_order_seq_cst, std::memory_order_relaxed
                        )) {
                        mBottom.store(bottom + 1, std::memory_order_relaxed);
                        return nullptr;
                    }
                    mBottom.store(bottom + 1, std::memory_order_relaxed);
                }
                return handle;
            }

            std::coroutine_handle<> steal() noexcept {
                size_t top = mTop.load(std::memory_order_acquire);
                std::atomic_thread_fence(std::memory_order_seq_cst);
                size_t bottom = mBottom.load(std::memory_order_acquire);
                if (top >= bottom) return nullptr;
                auto handle = mBuffer[top & (mCapacity - 1)];
                if (!mTop.compare_exchange_strong(top, top + 1, std::memory_order_seq_cst, std::memory_order_relaxed))
                    return nullptr;
                return handle;
            }

            bool empty() const noexcept {
                size_t top = mTop.load(std::memory_order_relaxed);
                size_t bottom = mBottom.load(std::memory_order_relaxed);
                return top >= bottom;
            }

        private:
            std::unique_ptr<std::coroutine_handle<>[]> mBuffer;
            const size_t                               mCapacity;
            alignas(CacheLineSize) std::atomic<size_t> mTop{0};
            alignas(CacheLineSize) std::atomic<size_t> mBottom{0};

            inline static thread_local std::vector<std::coroutine_handle<>> sTempBatchBuffer;
        };

    } // namespace detail

    class StaticThreadPool {
    public:
        explicit StaticThreadPool(
            size_t threadCount = std::thread::hardware_concurrency(),
            size_t queueCapacity = 64
        ) {
            if (threadCount == 0) threadCount = 1;
            mQueues.reserve(threadCount);
            mThreads.reserve(threadCount);
            for (size_t i = 0; i < threadCount; ++i) {
                mQueues.emplace_back(queueCapacity);
            }
            for (size_t i = 0; i < threadCount; ++i) {
                mThreads.emplace_back([this, i] { workerLoop(i); });
            }
        }

        ~StaticThreadPool() noexcept {
            mDone.store(true, std::memory_order_release);
            mCondition.notify_all();
            // std::jthread are automatically joined.
        }

        constexpr auto schedule() noexcept {
            struct ThreadPoolAwaiter {
                constexpr ThreadPoolAwaiter(StaticThreadPool &threadPool) noexcept : mThreadPool(threadPool) {}
                constexpr bool await_ready() const noexcept { return false; }
                constexpr void await_resume() const noexcept {}
                void           await_suspend(std::coroutine_handle<> handle) {
                    mThreadPool.submit(handle);
                }

            private:
                StaticThreadPool &mThreadPool;
            };
            return ThreadPoolAwaiter{*this};
        }

        void submit(std::coroutine_handle<> handle) {
            if (sLocalQueue) {
                if (sLocalQueue->push(handle))
                    return;
                // Local queue is full
            }
            // Not working thread
            mGlobalQueue.enqueue(handle);
            mCondition.notify_one();
        }

    private:
        std::coroutine_handle<> trySteal(size_t threadIndex) noexcept {
            size_t victimIndex = (threadIndex + 1) % mQueues.size();
            for (size_t i = 0; i < mQueues.size(); ++i) {
                if (victimIndex == threadIndex) {
                    victimIndex = (victimIndex + 1) % mQueues.size();
                    continue;
                }
                std::coroutine_handle<> handle = mQueues[victimIndex].steal();
                if (handle) return handle;

                victimIndex = (victimIndex + 1) % mQueues.size();
            }
            return {};
        }

        void workerLoop(size_t threadIndex) noexcept {
            sThreadIndex = threadIndex;
            sLocalQueue = &mQueues[threadIndex];

            while (!mDone.load(std::memory_order_acquire)) {
                std::coroutine_handle<> handle = sLocalQueue->pop();

                if (handle) {
                    handle.resume();
                    continue;
                }

                if (mGlobalQueue.try_dequeue(handle)) {
                    handle.resume();
                    continue;
                }

                if ((handle = trySteal(threadIndex))) {
                    handle.resume();
                    continue;
                }

                std::unique_lock lock(mMutex);
                mCondition.wait(lock, [this] {
                    return mDone.load(std::memory_order_relaxed) || !areAllQueuesEmpty();
                });
            }
        }

        bool areAllQueuesEmpty() const {
            for (const auto &queue : mQueues) {
                if (!queue.empty()) return false;
            }
            return mGlobalQueue.size_approx() == 0;
        }

        std::vector<detail::WorkStealingQueue>               mQueues;
        moodycamel::ConcurrentQueue<std::coroutine_handle<>> mGlobalQueue;
        struct alignas(CacheLineSize) {
            std::mutex              mMutex;
            std::condition_variable mCondition;
            std::atomic<bool>       mDone{false};
        };
        std::vector<std::jthread> mThreads;

        inline static thread_local detail::WorkStealingQueue *sLocalQueue = nullptr;
        inline static thread_local size_t                     sThreadIndex = std::numeric_limits<size_t>::max();
    };

} // namespace sapphire::coro
