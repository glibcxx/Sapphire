#pragma once

#include "SDK/api/src-client/common/module/IGameModuleApp.h"

class VanillaGameModuleApp : public IGameModuleApp {
public:
    SPHR_DECL_API("1.21.50", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x4D\x8B\xF1\x49\x8B\xE8\x48\x8B\xDA")
    SDK_API virtual std::unique_ptr<mce::framebuilder::FrameBuilder> createFrameBuilder(
        WorkerPool                                         &rendererWorkerPool,
        const gsl::span<std::reference_wrapper<WorkerPool>> helperPools,
        Scheduler                                          &scheduler
    ) override;
};