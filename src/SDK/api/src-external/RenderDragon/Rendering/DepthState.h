#pragma once

namespace dragon::rendering {

    enum class DepthComparisonFunction : int {
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

    struct DepthState {
        bool                    mEnabled;            // off+0
        bool                    mWriteDepth;         // off+1
        uint8_t                 mUnk2;               // off+2
        DepthComparisonFunction mComparisonFunction; // off+4
    };

}; // namespace dragon::rendering