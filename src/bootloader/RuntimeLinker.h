#pragma once

#include <memory>

#include "IatPatcher.h"
#include "SymbolResolver.h"

namespace sapphire::bootloader {

    class RuntimeLinker {
    public:
        explicit RuntimeLinker(const SymbolResolver &resolver, ipc::Client &IPCClient);
        ~RuntimeLinker();

        RuntimeLinker(const RuntimeLinker &) = delete;
        RuntimeLinker &operator=(const RuntimeLinker &) = delete;

        const SymbolResolver &getSymbolResolver() const { return mResolver; }
        IatPatcher           &getIatPatcher() const { return *mIatPatcher; }

        static void forceDllMainToFail(RuntimeLinker *self, HMODULE hDll);

    private:
        ipc::Client                &mIPCClient;
        const SymbolResolver       &mResolver;
        std::unique_ptr<IatPatcher> mIatPatcher;
    };

} // namespace sapphire::bootloader