#pragma once

#include <atomic>

namespace mce {

    // size: 4
    class PerFrameHandleTracker {
        std::atomic<uint16_t> mCheckCount; // off+0
        std::atomic<bool>     mIsValid;    // off+2
    };

} // namespace mce