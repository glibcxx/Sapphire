#include "SymbolResolver.h"

#include "common/MemoryScanning.hpp"

namespace sapphire {

    SymbolResolver::SymbolResolver(sapphire::bootloader::SymbolResolver &blsr) :
        mBLSymbolResolver(blsr) {}

    SymbolResolver &SymbolResolver::getInstance() {
        static SymbolResolver instance{bootloader::SymbolResolver::get()};
        return instance;
    }

} // namespace sapphire