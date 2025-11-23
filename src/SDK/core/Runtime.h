#pragma once

#include <Windows.h>
#include <Psapi.h>

#include <string>
#include "SymbolResolver.h"
#include "PluginManager.h"
#include "IPCClient.h"

namespace sapphire::core {

    class RenderBackend;

    class Runtime {
        PluginManager mPluginManager;
        IPCClient     mIPCClient;

        std::unique_ptr<RenderBackend> mRenderBackend;

        bool mInitialized = false;

        bool _init();

        void _shutdown() noexcept;

    public:
        Runtime() = default;
        ~Runtime() noexcept;
        Runtime(const Runtime &) = delete;
        Runtime &operator=(const Runtime &) = delete;

        SPHR_API static Runtime &getInstance();

        PluginManager &getPluginManager() {
            return this->mPluginManager;
        }

        void init();
        void shutdown() noexcept;
    };

} // namespace sapphire::core
