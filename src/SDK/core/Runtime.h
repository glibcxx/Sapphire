#pragma once

#include "common/coroutine/IoContext.hpp"
#include "pch.h" // IWYU pragma: keep

#include "common/IPC/Pipe.h"
#include "mod/ModRepository.h"
#include <optional>

namespace sapphire::core {

    class RenderBackend;
    class CrashLogger;

    class Runtime {
        ModRepository mModRepo;

        std::unique_ptr<RenderBackend>    mRenderBackend;
        std::unique_ptr<CrashLogger>      mCrashLogger;
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

        CrashLogger &getCrashLogger() const { return *mCrashLogger; }

        void init();
        void shutdown() noexcept;
    };

} // namespace sapphire::core
