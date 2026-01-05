#pragma once

#include "SDK/api/src/common/world/level/dimension/DimensionType.h"

class RenderChunkCoordinatorProxy;

// size: 64
struct LevelRendererCameraProxyCallbacks {
public:
    std::function<RenderChunkCoordinatorProxy *(DimensionType)> mGetRenderChunkCoordinator; // off+0
};

// size: 64
class LevelRendererCameraProxy {
public:
    LevelRendererCameraProxyCallbacks mCallbacks; // off+0
};