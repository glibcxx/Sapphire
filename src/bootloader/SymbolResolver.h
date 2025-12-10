#pragma once

#include <Windows.h>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
#include "SigDatabase.h"
#include "macros/Macros.h"
#include "util/DecoratedName.hpp"
#include "util/Signature.h"
#include "util/threading/ThreadPool.h"

namespace sapphire::bootloader {

    class SymbolResolver {
    public:
        using SymbolMap = std::unordered_map<std::string, uintptr_t>;

        SymbolResolver();
        ~SymbolResolver();

        SRBL_API static SymbolResolver &get();

        bool loadDatabase(const std::filesystem::path &dbPath);

        void resolve();

        const SymbolMap &getResolvedFunctionSymbols() const { return mResolvedFunctionSymbols; }
        SymbolMap       &getResolvedFunctionSymbols() { return mResolvedFunctionSymbols; }

        const SymbolMap &getResolvedDataSymbols() const { return mResolvedDataSymbols; }
        SymbolMap       &getResolvedDataSymbols() { return mResolvedDataSymbols; }

    private:
        static uintptr_t applyOperations(uintptr_t address, const std::vector<codegen::SigDatabase::SigOp> &ops);

        SymbolMap  mResolvedFunctionSymbols;
        SymbolMap  mResolvedDataSymbols;
        std::mutex mSymbolMapLock;

        std::unique_ptr<codegen::SigDatabase> mDatabase;
        util::ThreadPool                      mThreadPool;
    };

} // namespace sapphire::bootloader