#pragma once

#include <optional>
#include "common/IPC/Pipe.h"
#include "common/coroutine/IoContext.hpp"
#include "common/coroutine/Task.hpp"
#include "common/sys/MiniWindows.h"

namespace sapphire::bootloader {

    class SymbolResolver;
    class RuntimeLinker;

    class Bootloader {
        sys::win::hmodule_t                         mModule;
        std::unique_ptr<bootloader::SymbolResolver> mSymbolResolver;
        std::unique_ptr<bootloader::RuntimeLinker>  mRuntimeLinker;
        std::optional<coro::IoContext>              mIoCtx;
        std::optional<ipc::backend::Pipe>           mPipeConnection;

    public:
        Bootloader(sys::win::hmodule_t hModule);
        ~Bootloader();

        int run();

        coro::Task<bool> bootSapphire();
    };

} // namespace sapphire::bootloader