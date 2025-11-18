#pragma once

#include "SDK/api/src-client/common/client/input/HolographicPlatformInterface.h"

namespace OpenXR {

    // size: 1488
    class OpenXRPlatformInterface : public HolographicPlatform {
    public:
        uint8_t _fill888[1488 - sizeof(HolographicPlatform)]; // off+888
    };

} // namespace OpenXR
