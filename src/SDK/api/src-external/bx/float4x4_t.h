#pragma once

namespace bx {

    // size: 64
    struct alignas(16) float4x4_t {
        __m128 col[4];
    };

} // namespace bx