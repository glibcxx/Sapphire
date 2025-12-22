#pragma once

#include <Windows.h>
#include <Psapi.h>

#include <string>
#include "SymbolResolver.h"
#include "IPCClient.h"
#include "mod/ModRepository.h"

namespace sapphire::core {

    class RenderBackend;
    class CrashLogger;

    class Runtime {
        ModRepository mModRepo;
        IPCClient     mIPCClient;

        std::unique_ptr<RenderBackend> mRenderBackend;
        std::unique_ptr<CrashLogger>   mCrashLogger;

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
