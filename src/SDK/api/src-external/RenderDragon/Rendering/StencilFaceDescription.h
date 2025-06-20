#pragma once

namespace dragon::rendering {

    enum class StencilComparisonFunction : int {};

    enum class StencilOp : int {
        Keep = 0,
        Zero = 1,
        Replace = 2,
        Increment = 6,
        Decrement = 7,
    };

    struct StencilFaceDescription {
        StencilComparisonFunction mComparisonFunc; // off+0
        StencilOp                 mDepthFailOp;    // off+4
        StencilOp                 mPassOp;         // off+8
        StencilOp                 mFailOp;         // off+12
    };

}; // namespace dragon::rendering