#pragma once

namespace bgfx {

    // size: 1
    struct RenderFrame {
        enum class Enum : int {
            NoContext = 0,
            Render = 1,
            Timeout = 2,
            Exiting = 3,
            Count = 4,
        };
    };

} // namespace bgfx