#pragma once

#include "SDK/api/src/common/world/level/dimension/DimensionType.h"

class RenderChunkCoordinatorProxy;
namespace ClientBlockPipeline {
    class SchematicsRepository;
}

// size: 192
struct LevelRendererProxyCallbacks {
    std::function<RenderChunkCoordinatorProxy *(DimensionType)>                 mGetRenderChunkCoordinator; // off+0
    std::function<bool()>                                                       isBuildingRenderChunks;     // off+64
    std::function<std::shared_ptr<ClientBlockPipeline::SchematicsRepository>()> getSchematicsRepository;    // off+128
};

// size: 192
class LevelRendererProxy {
public:
    LevelRendererProxyCallbacks mCallbacks; // off+0
};
