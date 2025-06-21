#pragma once

#include <shared_mutex>
#include <unordered_map>

namespace Core {

    // size: 72
    template <typename Key, typename Val, typename Ptr>
    class Cache {
    public:
        using TValuePtr = Ptr;

        std::shared_mutex                  mAccess;  // off+0
        std::unordered_map<Key, TValuePtr> mContent; // off+8
    };

} // namespace Core