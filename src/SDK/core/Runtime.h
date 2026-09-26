#pragma once

#include "pch.h" // IWYU pragma: keep

#include "common/coroutine/IoContext.hpp"
#include "common/IPC/Pipe.h"
#include "mod/ModRepository.h"

namespace sapphire::core {

    class RenderBackend;

    class Runtime {
        ModRepository mModRepo;

        std::unique_ptr<RenderBackend>    mRenderBackend;
        std::optional<ipc::backend::Pipe> mPipeLogger;
        std::optional<coro::IoContext>    mIoCtx;

        bool mInitialized = false;

        bool _init();

        void _shutdown() noexcept;

    public:
        Runtime();
        ~Runtime() noexcept;
        Runtime(const Runtime &) = delete;
        Runtime &operator=(const Runtime &) = delete;

        SPHR_API static Runtime &getInstance();

        const ModRepository &getModRepository() const { return this->mModRepo; }
        ModRepository       &getModRepository() { return this->mModRepo; }

        void init();
        void shutdown() noexcept;
    };

} // namespace sapphire::core
