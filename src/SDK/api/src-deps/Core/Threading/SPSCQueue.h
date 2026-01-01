#pragma once

#include "atomicops.h"

template <typename T, size_t MAX_BLOCK_SIZE>
class SPSCQueue {
public:
    enum AllocationMode {
        CanAlloc = 0,
        CannotAlloc = 1,
    };

    struct Block {
    public:
        Lockless::WeakAtomic<size_t>  front;                // off+0
        size_t                        localTail;            // off+8
        char                          cachelineFiller0[48]; // off+16
        Lockless::WeakAtomic<size_t>  tail;                 // off+64
        size_t                        localFront;           // off+72
        char                          cachelineFiller1[48]; // off+80
        Lockless::WeakAtomic<Block *> next;                 // off+128
        char                         *data;                 // off+136
        const size_t                  sizeMask;             // off+144
        char                         *rawThis;              // off+152
    };

    Lockless::WeakAtomic<Block *> mFrontBlock;          // off+0
    char                          mCachelineFiller[56]; // off+8
    Lockless::WeakAtomic<Block *> mTailBlock;           // off+64
    size_t                        mLargestBlockSize;    // off+72
};