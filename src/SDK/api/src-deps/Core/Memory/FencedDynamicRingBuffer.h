#pragma once

#include <memory>
#include <optional>
#include <shared_mutex>
#include "SDK/api/src-deps/Core/Utility/brstd/flat_map.h"

// size: 192 (1.21.50)
template <typename TBuffer, size_t N>
class FencedRingBuffer {
public:
    using Fence = uint64_t;
    using OptionalFence = std::optional<Fence>;
    using BufferSize = uint64_t;
    using BufferOffset = uint64_t;

    alignas(8) uint8_t _fill[192];
};

template <typename TBuffer, size_t M = 2, size_t N = 0>
class FencedDynamicRingBuffer {
public:
    using Ring = FencedRingBuffer<TBuffer, N>;
    using RingPtr = std::shared_ptr<Ring>;
    using RingRawPtr = Ring *;

    using BufferSize = typename Ring::BufferSize;
    using BufferOffset = typename Ring::BufferOffset;
    using Fence = typename Ring::Fence;
    using OptionalFence = typename Ring::OptionalFence;
    // using OptionalScopedAllocation = std::optional<Ring::ScopedAllocation>;

    using RingMap = brstd::flat_map<BufferSize, std::shared_ptr<Ring>>;

    std::atomic<RingRawPtr *> mLastPageAllocatedFrom;        // off+0
    std::atomic<BufferSize>   mAllocationScopeCheck;         // off+8
    const BufferSize          mInitialSize;                  // off+16
    std::atomic<BufferSize>   mUsedSize;                     // off+24
    std::atomic<BufferSize>   mAllocationSize;               // off+32
    RingMap                   mRings;                        // off+40
    uint16_t                  mRingLifetimeWithNoAllocation; // off+96
    Fence                     mCurrentFence;                 // off+104
    std::optional<Fence>      mLastCompletedFenceValue;      // off+112
    std::shared_ptr<TBuffer>  mBufferImpl;                   // off+128
    std::shared_mutex         mAccessLock;                   // off+144
    int                       mUnk125;                       // off+152
};
