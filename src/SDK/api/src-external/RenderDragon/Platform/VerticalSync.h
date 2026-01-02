#pragma once

namespace dragon::platform {

    enum class VerticalSync : uint8_t {
        NoVerticalSync = 0,
        VerticalSync = 1,
        AdaptiveVerticalSync = 2,
    };

}