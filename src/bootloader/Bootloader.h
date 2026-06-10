#pragma once

#include "common/IPC/Client.h"
#include "common/sys/MiniWindows.h"

namespace sapphire::bootloader {

    class SymbolResolver;
    class RuntimeLinker;

    class Bootloader {
        sys::win::hmodule_t                         mModule;
        ipc::Client                                 mIPCClient;
        std::unique_ptr<bootloader::SymbolResolver> mSymbolResolver;
        std::unique_ptr<bootloader::RuntimeLinker>  mRuntimeLinker;

    public:
        Bootloader(sys::win::hmodule_t hModule);
        ~Bootloader();

        bool preBoot();

        void bootSapphire();

        void postBoot();
    };

} // namespace sapphire::bootloader