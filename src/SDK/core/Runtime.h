#pragma once

#include <Windows.h>
#include <Psapi.h>

#include <string>
#include "SymbolResolver.h"
#include "PluginManager.h"
#include "IPCClient.h"

namespace sapphire::core {

    class RenderBackend;
    class CrashLogger;

    class Runtime {
        PluginManager mPluginManager;
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

        const PluginManager &getPluginManager() const { return this->mPluginManager; }
        PluginManager       &getPluginManager() { return this->mPluginManager; }

        CrashLogger &getCrashLogger() const { return *mCrashLogger; }

        void init();
        void shutdown() noexcept;
    };

} // namespace sapphire::core
