#pragma once

#include "pch.h" // IWYU pragma: keep

namespace dragon::platform {

    enum class RenderAPI : uint8_t {
        Default = 0,
        D3D11 = 1,
        D3D12 = 2,
        D3D12RTX = 3,
        GNM = 4,
        AGC = 5,
        Metal = 6,
        Noop = 7,
        Nvn = 8,
        OpenGL = 9,
        OpenGLES = 10,
        Vulkan = 11,
    };

}