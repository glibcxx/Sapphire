#pragma once

namespace dragon::rendering {

    enum class DepthComparisonFunction : int;

    struct DepthState {
        bool                    mEnabled;            // off+0
        bool                    mWriteDepth;         // off+1
        uint8_t                 mUnk2;               // off+2
        DepthComparisonFunction mComparisonFunction; // off+4
    };

}; // namespace dragon::rendering