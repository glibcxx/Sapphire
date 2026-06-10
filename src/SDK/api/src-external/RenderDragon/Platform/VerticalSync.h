#pragma once

#include "pch.h" // IWYU pragma: keep

namespace dragon::platform {

    enum class VerticalSync : uint8_t {
        NoVerticalSync = 0,
        VerticalSync = 1,
        AdaptiveVerticalSync = 2,
    };

}