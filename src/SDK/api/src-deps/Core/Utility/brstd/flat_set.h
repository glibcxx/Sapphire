#pragma once

#include <set>

namespace brstd {

    template <
        typename Kty,
        typename Pred = std::less<Kty>,
        typename KtyCtn = std::vector<Kty>>
    class flat_set {
        KtyCtn   mKeys;    // off+0
        uint64_t _fill[2]; // off+24
    };

} // namespace brstd