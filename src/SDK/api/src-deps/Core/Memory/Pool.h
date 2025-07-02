#pragma once

#include <functional>
#include "SDK/api/src-deps/Core/Threading/MPMCQueue.h"

// size: 696
template <typename Ty>
class Pool {
public:
    // size: 1
    class AccessTag {};

    using LifetimeTag = std::weak_ptr<AccessTag>;

    using Ptr = std::shared_ptr<Ty>;

    std::shared_ptr<AccessTag>     mAccessTag; // off+0
    MPMCQueue<std::unique_ptr<Ty>> mPool;      // off+16
    std::function<Ty *()>          mAllocator; // off+632

    // \xE8\x00\x00\x00\x00\x48\x8B\x08\x4C\x89\x20\x48\x8B\xB5 1.21.50
    static std::unique_ptr<Pool> fromCustomAllocator(std::function<Ty *()> allocator);
};
static_assert(sizeof(Pool<int>) == 696);