#pragma once

#include <cmath>

namespace mce {

    class Math {
    public:
        template <typename T>
        static T sign(T input) {
            if (input > 0.0f)
                return 1;
            if (input < 0.0f)
                return -1;
            return 0;
        }

        template <typename T1, typename T2>
        static auto max(T1 a, T2 b) -> std::remove_reference_t<decltype(a > b ? a : b)> {
            return a > b ? a : b;
        }

        template <typename T1, typename T2>
        static auto min(T1 a, T2 b) -> std::remove_reference_t<decltype(a < b ? a : b)> {
            return a < b ? a : b;
        }

        template <typename T>
        static T clamp(T v, T low, T high) {
            if (v > high)
                return high;
            if (v <= low)
                return low;
            else
                return v;
        }

        template <typename T>
        static int round(T val) {
            return (int)std::round(val);
        }

        template <typename T>
        static size_t hash_accumulate(size_t hash, const T &toHash) {
            return ((hash >> 2) + (hash << 6) + std::hash<T>{}(toHash) + 2654435769u) ^ hash;
        }

        template <typename T1, typename T2>
        static size_t hash2(const T1 &a, const T2 &b) {
            size_t hash = mce::Math::hash_accumulate<T1>(0LL, a);
            return mce::Math::hash_accumulate<T2>(hash, b);
        }

        template <typename T1, typename T2, typename T3>
        static size_t hash3(const T1 &a, const T2 &b, const T3 &c) {
            size_t hash = mce::Math::hash_accumulate<T2>(mce::Math::hash_accumulate<T1>(0, a), b);
            return mce::Math::hash_accumulate<T3>(hash, c);
        }

        template <typename T>
        static auto square(const T &n) -> std::remove_reference_t<decltype(n * n)> {
            return n * n;
        }
    };

} // namespace mce