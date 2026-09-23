#pragma once

#include <memory>

#include "IatPatcher.h"
#include "SymbolResolver.h"
#include "common/IPC/PipeChannel.h"
#include "common/sys/MiniWindows.h"

namespace sapphire::bootloader {

    class RuntimeLinker {
    public:
        explicit RuntimeLinker(const SymbolResolver &resolver, ipc::PipeChannel log);
        ~RuntimeLinker();

        RuntimeLinker(const RuntimeLinker &) = delete;
        RuntimeLinker &operator=(const RuntimeLinker &) = delete;

        const SymbolResolver &getSymbolResolver() const { return mResolver; }
        IatPatcher           &getIatPatcher() const { return *mIatPatcher; }

        static void forceDllMainToFail(RuntimeLinker *self, sys::win::hmodule_t hDll);

    private:
        ipc::PipeChannel            mPipeLogger;
        const SymbolResolver       &mResolver;
        std::unique_ptr<IatPatcher> mIatPatcher;
    };

} // namespace sapphire::bootloader