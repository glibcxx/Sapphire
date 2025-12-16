#pragma once

#include "common/IPC/Client.h"

namespace sapphire::bootloader {

    class SymbolResolver;
    class RuntimeLinker;

    class Bootloader {
        HMODULE                                     mModule;
        ipc::Client                                 mIPCClient;
        std::unique_ptr<bootloader::SymbolResolver> mSymbolResolver;
        std::unique_ptr<bootloader::RuntimeLinker>  mRuntimeLinker;

    public:
        Bootloader(HMODULE hModule);
        ~Bootloader();

        bool preBoot();

        void bootSapphire();

        void postBoot();
    };

} // namespace sapphire::bootloader