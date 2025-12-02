#pragma once

#include <Windows.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "SigDatabase.h"

namespace sapphire::bootloader {

    class SymbolResolver {
    public:
        // Tries to find and load the signature database file for the current game version.
        bool loadDatabase(const std::filesystem::path &dbPath);

        // Scans all signatures and resolves them to final addresses.
        void resolve();

        // Returns the map of resolved symbol names to their absolute addresses.
        const std::unordered_map<std::string, uintptr_t> &getResolvedSymbols() const;

    private:
        uintptr_t applyOperations(uintptr_t address, const std::vector<codegen::SigDatabase::SigOp> &ops);

        std::unique_ptr<codegen::SigDatabase>      mDatabase;
        std::unordered_map<std::string, uintptr_t> mResolvedSymbols;
    };

} // namespace sapphire::bootloader