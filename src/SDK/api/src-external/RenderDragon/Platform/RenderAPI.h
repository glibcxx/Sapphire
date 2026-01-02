#pragma once

namespace dragon::platform {

    enum class RenderAPI : uint8_t {
        Default = 0,
        D3D11 = 1,
        D3D12 = 2,
        D3D12RTX = 3,
        GNM = 4,
        Metal = 5,
        Noop = 6,
        NVN = 7,
        OpenGL = 8,
        OpenGLES = 9,
        Vulkan = 10,
    };

}