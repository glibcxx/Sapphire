#pragma once

#include "pch.h" // IWYU pragma: keep

namespace dragon::rendering {

    enum class ClipSpaceOrigin : uint8_t {
        TopLeft = 0,
        BottomLeft = 1,
    };

} // namespace dragon::rendering