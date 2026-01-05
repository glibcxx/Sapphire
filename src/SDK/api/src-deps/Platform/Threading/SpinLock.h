#pragma once

// size: 32
class SpinLock {
public:
    std::hash<std::thread::id> mThreadHasher;  // off+0
    const uint64_t             mNoThreadId;    // off+8
    std::atomic<uint64_t>      mOwnerThread;   // off+16
    uint32_t                   mOwnerRefCount; // off+24
};