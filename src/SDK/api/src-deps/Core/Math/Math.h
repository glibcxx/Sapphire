#pragma once

namespace mce {

    class Math {
    public:
        template <typename T>
        static size_t hash_accumulate(size_t hash, const T &toHash) {
            return ((hash >> 2) + (hash << 6) + std::hash<T>{}(toHash) + 2654435769u) ^ hash;
        }
    };

} // namespace mce