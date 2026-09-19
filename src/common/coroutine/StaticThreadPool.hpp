#pragma once

#include "macros/Macros.h"

#include <atomic>
#include <coroutine>
#include <vector>
#include <thread>
#include <memory>
#include <cassert>
#include <mutex>

#include "common/sys/MiniWindows.h"

#include "Constant.hpp"

#pragma push_macro("CPU_PAUSE")

#undef CPU_PAUSE
#if defined(_WIN32_WINNT) || defined(_WIN32)
#    include <intrin.h>
#    define CPU_PAUSE() _mm_pause()
#elif defined(__GNUC__) || defined(__clang__)
#    define CPU_PAUSE() __builtin_ia32_pause()
#else
#    define CPU_PAUSE() std::this_thread::yield()
#endif

namespace sapphire::coro {

    namespace detail {

        // msvc STL std::mutex 的替代
        class mutex {
        public:
            using native_handle_type = void *;

            constexpr mutex() noexcept : mLock(nullptr) {}

            ~mutex() = default;

            mutex(const mutex &) = delete;
            mutex &operator=(const mutex &) = delete;

            void lock() { AcquireSRWLockExclusive((PSRWLOCK)&mLock); }

            bool try_lock() noexcept { return TryAcquireSRWLockExclusive((PSRWLOCK)&mLock) != 0; }

            void unlock() noexcept { ReleaseSRWLockExclusive((PSRWLOCK)&mLock); }

            native_handle_type native_handle() noexcept { return &mLock; }

        private:
            void *mLock;
        };

        class AutoResetEvent {
            void *mEvent{nullptr};

        public:
            AutoResetEvent() { mEvent = CreateEventW(nullptr, 0, 0, nullptr); }

            ~AutoResetEvent() noexcept {
                if (mEvent) CloseHandle(mEvent);
            }

            AutoResetEvent &operator=(const AutoResetEvent &) = delete;

            void set() noexcept { SetEvent(mEvent); }

            void wait() noexcept { WaitForSingleObjectEx(mEvent, 0xffffffff, 0); }
        };

        class alignas(CacheLineSize) SegmentedLaneBase {
        public:
            static constexpr size_t BLOCK_CAPACITY = (2 << 5) - 1;

            struct alignas(CacheLineSize) Block {
                std::coroutine_handle<> data[BLOCK_CAPACITY];
                Block                  *next{nullptr};
            };

            enum class WorkerWaitState : char { Running = 0,
                                                PendingWait,
                                                Waiting };

        protected:
            alignas(CacheLineSize) std::mutex mMutex;
            std::coroutine_handle<> mRunNext{};

            Block *mHeadBlock{nullptr};
            Block *mTailBlock{nullptr};
            Block *mFreeTail{nullptr};

            size_t mHeadOffset{0};
            size_t mTailOffset{0};
            size_t mFreeBlockCount{0};

            alignas(CacheLineSize) std::atomic<size_t> mSizeApprox{0};

        public:
            alignas(CacheLineSize) detail::AutoResetEvent mWorkerWait;
            std::atomic<WorkerWaitState> mWorkerState;
        };

    } // namespace detail

    template <
        typename Allocator = std::allocator<std::coroutine_handle<>>,
        typename BlockAllocator = std::allocator_traits<Allocator>::template rebind_alloc<detail::SegmentedLaneBase::Block>>
    class SegmentedLane : protected BlockAllocator, public detail::SegmentedLaneBase {
    public:
        static constexpr size_t MAX_FREE_BLOCK_COUNT = 8;

        using BlockTraits = std::allocator_traits<BlockAllocator>;

        explicit SegmentedLane(const Allocator &alloc = Allocator()) : BlockAllocator(alloc) {
            Block *newBlock = BlockTraits::allocate(allocator(), 1);
            BlockTraits::construct(allocator(), newBlock);

            newBlock->next = nullptr;
            mHeadBlock = mTailBlock = mFreeTail = newBlock;
        }

        ~SegmentedLane() noexcept {
            Block *curr = mHeadBlock;
            while (curr != nullptr) {
                Block *nxt = curr->next;
                destroyBlock(curr);
                curr = nxt;
            }
        }

        SegmentedLane &operator=(const SegmentedLane &) = delete;

        bool pushRunNext(std::coroutine_handle<> h) {
            if (!mRunNext) [[likely]] {
                mRunNext = h;
                return true;
            }
            push(h);
            return false;
        }

        bool                    hasRunNext() const noexcept { return (bool)mRunNext; }
        std::coroutine_handle<> popRunNext() noexcept { return std::exchange(mRunNext, nullptr); }

        void push(std::coroutine_handle<> h) {
            std::lock_guard lock(mMutex);

            if (mTailOffset < BLOCK_CAPACITY) [[likely]] {
                mTailBlock->data[mTailOffset++] = h;
                mSizeApprox.fetch_add(1, std::memory_order_relaxed);
                return;
            }
            if (mTailBlock->next != nullptr) [[likely]] {
                mTailBlock = mTailBlock->next;
                mTailBlock->data[0] = h;
                mTailOffset = 1;
                mSizeApprox.fetch_add(1, std::memory_order_relaxed);
                return;
            }

            allocBlockAndPush(h);
        }

        std::coroutine_handle<> pop() noexcept {
            std::lock_guard lock(mMutex);

            if (mHeadBlock == mTailBlock && mHeadOffset == mTailOffset) return nullptr;

            auto h = mHeadBlock->data[mHeadOffset++];
            mSizeApprox.fetch_sub(1, std::memory_order_relaxed);

            if (mHeadOffset < BLOCK_CAPACITY) [[likely]]
                return h;

            stepToNextBlockAndTryShrink();
            return h;
        }

        size_t stealBatch(std::coroutine_handle<> *destBuf, size_t maxSteal) noexcept {
            if (mSizeApprox.load(std::memory_order_relaxed) == 0) { return 0; }

            std::unique_lock lock(mMutex, std::try_to_lock);
            if (!lock.owns_lock()) return 0;

            if (mHeadBlock == mTailBlock && mHeadOffset == mTailOffset) {
                mHeadOffset = 0;
                mTailOffset = 0;
                return 0;
            }

            size_t approx = mSizeApprox.load(std::memory_order_relaxed);
            if (approx == 0) return 0;

            size_t toSteal = std::min((approx + 1) / 2, maxSteal);
            size_t stolen = 0;

            while (stolen < toSteal) {
                size_t inCurr =
                    (mHeadBlock == mTailBlock) ? (mTailOffset - mHeadOffset) : (BLOCK_CAPACITY - mHeadOffset);
                size_t take = std::min(inCurr, toSteal - stolen);

                std::memcpy(&destBuf[stolen], &mHeadBlock->data[mHeadOffset], take * sizeof(std::coroutine_handle<>));

                mHeadOffset += take;
                stolen += take;

                if (mHeadOffset == BLOCK_CAPACITY) {
                    stepToNextBlockAndTryShrink();
                } else {
                    break;
                }
            }

            mSizeApprox.fetch_sub(stolen, std::memory_order_relaxed);
            return stolen;
        }

        [[nodiscard]] size_t approxSize() const noexcept { return mSizeApprox.load(std::memory_order_relaxed); }

    private:
        const BlockAllocator &allocator() const noexcept { return *this; }
        BlockAllocator       &allocator() noexcept { return *this; }

        SPHR_FORCE_INLINE void allocBlockAndPush(std::coroutine_handle<> h) {
            Block *newBlock = BlockTraits::allocate(allocator(), 1);
            BlockTraits::construct(allocator(), newBlock);

            newBlock->next = nullptr;
            mTailBlock->next = newBlock;
            mTailBlock = newBlock;
            mFreeTail = newBlock;

            mTailBlock->data[0] = h;
            mTailOffset = 1;
            mSizeApprox.fetch_add(1, std::memory_order_relaxed);
        }

        SPHR_FORCE_INLINE void stepToNextBlockAndTryShrink() noexcept {
            if (mHeadBlock != mTailBlock) {
                Block *finishedBlock = mHeadBlock;
                mHeadBlock = mHeadBlock->next;
                mHeadOffset = 0;

                if (mFreeBlockCount < MAX_FREE_BLOCK_COUNT) [[likely]] {
                    finishedBlock->next = nullptr;
                    mFreeTail->next = finishedBlock;
                    mFreeTail = finishedBlock;
                    ++mFreeBlockCount;
                } else [[unlikely]] {
                    destroyBlock(finishedBlock);
                }
            } else {
                mHeadOffset = 0;
                mTailOffset = 0;
            }
        }

        void destroyBlock(Block *b) noexcept {
            BlockTraits::destroy(allocator(), b);
            BlockTraits::deallocate(allocator(), b, 1);
        }
    };

    class MultiLaneThreadPool {
        class FastRand {
            inline static std::atomic<uint64_t> sSeedSeq{0};

            uint64_t mState;

            static uint64_t mix(uint64_t x) noexcept {
                x ^= x >> 30;
                x *= 0xbf58476d1ce4e5b9ULL;
                x ^= x >> 27;
                x *= 0x94d049bb133111ebULL;
                x ^= x >> 31;
                return x;
            }

        public:
            explicit FastRand() noexcept {
                uint64_t rawSeed = sSeedSeq.fetch_add(1, std::memory_order_relaxed);
                rawSeed ^= static_cast<uint64_t>(std::chrono::steady_clock::now().time_since_epoch().count());
                mState = mix(rawSeed ^ reinterpret_cast<uintptr_t>(&rawSeed));
                if (mState == 0) mState = 0x85ebca6bULL;
            }

            constexpr uint32_t operator()() noexcept { return nextU32(); }

            constexpr uint32_t nextU32() noexcept {
                mState += 0x9e3779b97f4a7c15ULL;
                uint64_t z = mState;
                z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
                z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
                return static_cast<uint32_t>(z ^ (z >> 31));
            }

            constexpr uint32_t nextBounded(uint32_t n) noexcept {
                return static_cast<uint32_t>((static_cast<uint64_t>(nextU32()) * n) >> 32);
            }
        };

        struct alignas(CacheLineSize) WorkerContext {
            FastRand                                mPRNG;
            size_t                                  mLocalBatchEndIdx = 0;
            std::array<std::coroutine_handle<>, 16> mLocalBatch;

            WorkerContext() = default;
        };

        inline static std::atomic<size_t> sWorkerThreadUniqueId{0};

    public:
        explicit MultiLaneThreadPool(
            size_t threadCount = std::thread::hardware_concurrency(), bool useThreadAffinity = false
        ) :
            mNumLanes(std::max<size_t>(1, threadCount)), mLanes(mNumLanes), mWorkers(mNumLanes) {
            mThreads.reserve(mNumLanes);
            for (size_t i = 0; i < mNumLanes; ++i) {
                size_t uniqueId = sWorkerThreadUniqueId.fetch_add(1, std::memory_order_relaxed);
                mThreads.emplace_back([this, i, uniqueId, threadCount, useThreadAffinity] {
                    unsigned __int64 mask = 1 << ((uniqueId % threadCount) % 64);
                    auto             hThread = GetCurrentThread();
                    if (useThreadAffinity) SetThreadAffinityMask(hThread, mask);
                    SetThreadDescription(hThread, std::format(L"MultiLaneThreadPool - {} - {}", uniqueId, i).c_str());
                    workerLoop(i);
                });
            }
        }

        ~MultiLaneThreadPool() {
            mDone.store(true, std::memory_order_release);
            for (auto &lane : mLanes) lane.mWorkerWait.set();
            for (auto &t : mThreads) {
                if (t.joinable()) t.join();
            }
        }

        inline static thread_local FastRand tFastRand{};

        inline static thread_local SegmentedLane<> *tLocalLane = nullptr;

        void submit(std::coroutine_handle<> handle) {
            if (tLocalLane) [[unlikely]] {
                /*
                    有时一个异步 io 接口会直接成功返回，这意味着此次调用没有把协程调度到io线程上
                    ```cpp
                    co_await tp.schedule();
                    // ...
                    co_await socket.send(...);
                    co_await tp.schedule();
                    ```
                    也就意味着当前线程还是原来的线程，那么schedule实际上不需要真正入队
                    只需要一个单独的run-next变量专门保存就能降低队列压力
                */
                if (!tLocalLane->pushRunNext(handle)) [[unlikely]] {
                    uint32_t pick = tFastRand.nextBounded(mNumLanes);
                    auto    &lane = mLanes[pick];
                    if (lane.mWorkerState.exchange(
                            detail::SegmentedLaneBase::WorkerWaitState::Running, std::memory_order_seq_cst
                        )
                        == detail::SegmentedLaneBase::WorkerWaitState::Waiting) {
                        lane.mWorkerWait.set();
                    }
                }
                return;
            }
            uint32_t pick = tFastRand.nextBounded(mNumLanes);
            auto    &lane = mLanes[pick];
            lane.push(handle);

            if (lane.mWorkerState.exchange(detail::SegmentedLaneBase::WorkerWaitState::Running, std::memory_order_seq_cst)
                == detail::SegmentedLaneBase::WorkerWaitState::Waiting) {
                lane.mWorkerWait.set();
            }
        }

        struct [[nodiscard]] ScheduleAwaiter {
            MultiLaneThreadPool &pool;

            constexpr bool await_ready() const noexcept { return false; }
            void           await_suspend(std::coroutine_handle<> h) const { pool.submit(h); }
            constexpr void await_resume() const noexcept {}
        };

        ScheduleAwaiter schedule() noexcept { return ScheduleAwaiter{*this}; }

    private:
        bool stealWork(size_t selfId, WorkerContext &ctx) {
            const size_t n = mNumLanes;
            if (n <= 1) return false;
            if (ctx.mLocalBatchEndIdx) return true;

            uint32_t victimA = ctx.mPRNG.nextBounded(n);
            if (victimA == selfId) victimA = (victimA + 1 == n) ? 0 : victimA + 1;

            uint32_t victimB = ctx.mPRNG.nextBounded(n);
            if (victimB == selfId || victimB == victimA) {
                victimB = (victimA + 1 == n) ? 0 : victimA + 1;
                if (victimB == selfId) victimB = (victimB + 1 == n) ? 0 : victimB + 1;
            }

            const size_t pick = (mLanes[victimA].approxSize() >= mLanes[victimB].approxSize()) ? victimA : victimB;

            if ((ctx.mLocalBatchEndIdx = mLanes[pick].stealBatch(ctx.mLocalBatch.data(), ctx.mLocalBatch.size()))) {
                return true;
            }
            return false;
        }

        void workerLoop(size_t workerId) {
            WorkerContext   &ctx = mWorkers[workerId];
            SegmentedLane<> &myLane = mLanes[workerId];
            tLocalLane = &myLane;

            std::coroutine_handle<> handle{};

            while (!mDone.load(std::memory_order_acquire)) {
                if (myLane.hasRunNext()) {
                    myLane.popRunNext().resume();
                    continue;
                }

                handle = myLane.pop();
                if (handle) {
                    handle.resume();
                    continue;
                }

                if (stealWork(workerId, ctx)) {
                    for (; ctx.mLocalBatchEndIdx;) {
                        auto h = ctx.mLocalBatch[--ctx.mLocalBatchEndIdx];
                        h.resume();
                    }
                    continue;
                }

                bool               found = false;
                constexpr uint32_t SPIN_LIMITS = 48;
                for (uint32_t spin = 0; spin < SPIN_LIMITS; ++spin) {
                    for (uint32_t i = 0; i < 4 * ((spin >> 2) + 1); ++i) { CPU_PAUSE(); }
                    if (myLane.approxSize() > 0) {
                        found = true;
                        break;
                    }
                }
                if (found) continue;

                if (stealWork(workerId, ctx)) continue;

                auto &state = myLane.mWorkerState;
                state.store(detail::SegmentedLaneBase::WorkerWaitState::PendingWait, std::memory_order_seq_cst);
                if (myLane.approxSize() > 0 || stealWork(workerId, ctx) || mDone.load(std::memory_order_seq_cst)) {
                    state.store(detail::SegmentedLaneBase::WorkerWaitState::Running, std::memory_order_relaxed);
                    continue;
                }
                state.store(detail::SegmentedLaneBase::WorkerWaitState::Waiting, std::memory_order_seq_cst);
                if (myLane.approxSize() > 0 || stealWork(workerId, ctx) || mDone.load(std::memory_order_seq_cst)) {
                    state.store(detail::SegmentedLaneBase::WorkerWaitState::Running, std::memory_order_relaxed);
                    continue;
                }
                myLane.mWorkerWait.wait();
            }
        }

        const size_t                 mNumLanes;
        std::vector<SegmentedLane<>> mLanes;
        std::vector<WorkerContext>   mWorkers;
        std::vector<std::jthread>    mThreads;

        alignas(CacheLineSize) std::atomic<bool> mDone{false};
    };

    using StaticThreadPool = MultiLaneThreadPool;

} // namespace sapphire::coro

#pragma pop_macro("CPU_PAUSE") // NOLINT