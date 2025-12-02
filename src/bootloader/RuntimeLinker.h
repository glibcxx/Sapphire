#pragma once

#include <memory>

#include "IatPatcher.h"
#include "SymbolResolver.h"
#include "LogBox.h"

namespace sapphire::bootloader {

    class RuntimeLinker {
    public:
        explicit RuntimeLinker(const SymbolResolver &resolver);
        ~RuntimeLinker();

        RuntimeLinker(const RuntimeLinker &) = delete;
        RuntimeLinker &operator=(const RuntimeLinker &) = delete;

        const SymbolResolver &getSymbolResolver() const { return mResolver; }
        IatPatcher           &getIatPatcher() const { return *mIatPatcher; }

    private:
        const SymbolResolver       &mResolver;
        std::unique_ptr<IatPatcher> mIatPatcher;
    };

} // namespace sapphire::bootloader