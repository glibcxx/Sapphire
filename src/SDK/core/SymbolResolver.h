#pragma once

#include "macros/Macros.h"
#include "bootloader/SymbolResolver.h"

namespace sapphire {

    class SymbolResolver {
        using ApiMap = std::unordered_map<std::string, uintptr_t>;

        bootloader::SymbolResolver &mBLSymbolResolver;

        mutable std::mutex mLock;

        SymbolResolver(bootloader::SymbolResolver &blsr);

    public:
        SPHR_API static SymbolResolver &getInstance();

        const ApiMap &getFunctionApiMap() const {
            return mBLSymbolResolver.getResolvedFunctionSymbols();
        }
        const ApiMap &getDataApiMap() const {
            return mBLSymbolResolver.getResolvedDataSymbols();
        }

        uintptr_t findTarget(std::string_view dName) {
            std::unique_lock lock{mLock};

            auto &fm = mBLSymbolResolver.getResolvedFunctionSymbols();
            auto  it = fm.find(std::string{dName});
            if (it == fm.end())
                return 0;
            else
                return it->second;
        }

        template <auto ptr>
        uintptr_t findTarget() {
            return findTarget(util::Decorator<ptr, true, true>::value.view());
        }
    };

} // namespace sapphire
