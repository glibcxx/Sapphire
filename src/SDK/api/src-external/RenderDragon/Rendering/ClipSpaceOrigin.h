#pragma once

#include <cstdint>

namespace dragon::rendering {

    enum class ClipSpaceOrigin : uint8_t {
        TopLeft = 0,
        BottomLeft = 1,
    };

} // namespace dragon::rendering