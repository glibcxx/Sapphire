#pragma once

#include <vector>

namespace brstd /*bedrock_std*/ {

    // size: 56 (vector container)
    template <
        typename Kty,
        typename Ty,
        typename KComp = std::less<Kty>,
        typename KtyCtn = std::vector<Kty>,
        typename TyCtn = std::vector<Ty>>
    class flat_map {
    public:
        KtyCtn   mKeys; // off+0
        TyCtn    mVals; // off+24
        uint64_t mUnk;  // off+48

        constexpr Ty *find(const Kty &key) {
            auto range = std::equal_range(mKeys.begin(), mKeys.end(), key, KComp{});
            if (range.first == range.second) {
                return nullptr;
            }
            auto index = std::distance(mKeys.begin(), range.first);
            return &mVals[index];
        }

        constexpr size_t size() const { return mKeys.size(); }
        constexpr bool   empty() const { return mKeys.empty(); }
    };

} // namespace brstd