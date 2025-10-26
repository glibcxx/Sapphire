#pragma once

#include "SDK/api/src/common/world/module/IGameModuleShared.h"

class GameModuleClient;
class WorkerPool;
class Scheduler;
namespace mce::framebuilder {
    class FrameBuilder;
}

// size: 8
class IGameModuleApp : public IGameModuleShared {
public:
    // vtb+5
    virtual std::unique_ptr<GameModuleClient> createGameModuleClient() = 0;

    // vtb+6
    virtual std::unique_ptr<mce::framebuilder::FrameBuilder> createFrameBuilder(
        WorkerPool                                         &rendererWorkerPool,
        const gsl::span<std::reference_wrapper<WorkerPool>> helperPools,
        Scheduler                                          &scheduler
    ) = 0;

    // ...
};