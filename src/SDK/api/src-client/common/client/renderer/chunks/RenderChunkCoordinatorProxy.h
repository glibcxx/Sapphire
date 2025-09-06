#pragma once

#include <functional>
#include "SDK/api/src-deps/Core/Math/Color.h"

// size: 192
struct RenderChunkCoordinatorProxyCallbacks {
    std::function<size_t()>       mGetRenderChunkSharedCount; // off+0
    std::function<mce::Color &()> getFogColor;                // off+64
    std::function<float()>        getFogDistance;             // off+128
};

// size: 192
class RenderChunkCoordinatorProxy {
public:
    RenderChunkCoordinatorProxyCallbacks mCallbacks; // off+0
};