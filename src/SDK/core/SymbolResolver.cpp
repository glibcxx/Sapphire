#include "SymbolResolver.h"

#include "util/MemoryScanning.hpp"

namespace sapphire {

    uintptr_t SymbolResolver::_scanApi(const char *sig, size_t sigLength) {
        return memory::scanSignature(
            this->mMainModuleInfo.mTextSectionStart,
            this->mMainModuleInfo.mTextSectionSize,
            sig,
            sigLength
        );
    }

    SymbolResolver::SymbolResolver(sapphire::bootloader::SymbolResolver &blsr) :
        mBLSymbolResolver(blsr) {}

    SymbolResolver &SymbolResolver::getInstance() {
        static SymbolResolver instance{bootloader::SymbolResolver::get()};
        return instance;
    }

} // namespace sapphire