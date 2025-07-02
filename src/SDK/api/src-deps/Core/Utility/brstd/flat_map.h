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
        KtyCtn   mKeys; // off+0
        TyCtn    mVals; // off+24
        uint64_t mUnk;  // off+48
    };

} // namespace brstd