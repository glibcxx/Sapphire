#pragma once

#include "ValidityCheckType.h"
#include <atomic>

namespace mce {

    // size: 4
    class PerFrameHandleTracker {
    public:
        std::atomic<uint16_t> mCheckCount; // off+0
        std::atomic<bool>     mIsValid;    // off+2

        SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x80\x78\x01\x01\x75\x00\xB0\x01\x48\x8B\x5C\x24\x38")
        SDK_API std::pair<bool, bool> isValid(ValidityCheckType validityCheckType) const;
    };

} // namespace mce