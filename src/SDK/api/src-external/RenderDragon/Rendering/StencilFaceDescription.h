#pragma once

namespace dragon::rendering {

    enum class StencilComparisonFunction : int {
        None = 0,
        Never = 1,
        Less = 2,
        Equal = 3,
        LessEqual = 4,
        Greater = 5,
        NotEqual = 6,
        GreaterEqual = 7,
        Always = 8,
    };

    enum class StencilOp : int {
        Keep = 0,
        Zero = 1,
        Replace = 2,
        Increment = 6,
        Decrement = 7,
    };

    struct StencilFaceDescription {
        StencilComparisonFunction mComparisonFunc = StencilComparisonFunction::Always; // off+0
        StencilOp                 mDepthFailOp = StencilOp::Keep;                      // off+4
        StencilOp                 mPassOp = StencilOp::Keep;                           // off+8
        StencilOp                 mFailOp = StencilOp::Keep;                           // off+12
    };

}; // namespace dragon::rendering