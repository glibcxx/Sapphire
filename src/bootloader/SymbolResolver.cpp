#include "SymbolResolver.h"
#include <Windows.h>
#include <cassert>
#include <fstream>
#include <filesystem>
#include <Psapi.h>

#include "util/DecoratedName.hpp"
#include "util/Memory.hpp"
#include "util/MemoryScanning.hpp"

namespace sapphire::bootloader {

    static SymbolResolver *srInstance = nullptr;

    SymbolResolver::SymbolResolver() {
        srInstance = this;
    }
    SymbolResolver::~SymbolResolver() {
        srInstance = nullptr;
    }

    SymbolResolver &SymbolResolver::get() {
        assert(srInstance && "SymbolResolver is not available!");
        return *srInstance;
    }

    uintptr_t SymbolResolver::applyOperations(uintptr_t address, const std::vector<codegen::SigDatabase::SigOp> &ops) {
        uintptr_t currentAddress = address;
        for (const auto &op : ops) {
            switch (op.opType) {
            case codegen::SigDatabase::SigOpType::Disp:
                currentAddress += op.data;
                break;
            case codegen::SigDatabase::SigOpType::Deref:
                currentAddress = *reinterpret_cast<uintptr_t *>(currentAddress);
                break;
            case codegen::SigDatabase::SigOpType::Call:
                currentAddress = memory::deRef(currentAddress, memory::AsmOperation::CALL);
                break;
            case codegen::SigDatabase::SigOpType::Mov:
                currentAddress = memory::deRef(currentAddress, memory::AsmOperation::MOV);
                break;
            case codegen::SigDatabase::SigOpType::Lea:
                currentAddress = memory::deRef(currentAddress, memory::AsmOperation::LEA);
                break;
            default:
                break;
            }
        }
        return currentAddress;
    }

    bool SymbolResolver::loadDatabase(const std::filesystem::path &dbPath) {
        std::ifstream dbFile(dbPath, std::ios::binary);
        if (!dbFile.is_open()) {
            return false;
        }

        mDatabase = std::make_unique<codegen::SigDatabase>(0);
        if (!mDatabase->load(dbFile)) {
            mDatabase.reset();
            return false;
        }

        return true;
    }

    void SymbolResolver::resolve() {
        if (!mDatabase) {
            return;
        }

        const auto &entries = mDatabase->getSigEntries();

        HMODULE hModule = GetModuleHandle(nullptr);
        if (!hModule) return;

        MODULEINFO moduleInfo;
        if (!GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(moduleInfo))) {
            return;
        }

        const uintptr_t moduleBase = reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll);
        const size_t    moduleSize = moduleInfo.SizeOfImage;
        mResolvedFunctionSymbols[util::Decorator<&SymbolResolver::get>::value.value] = (uintptr_t)&SymbolResolver::get;
        mThreadPool.start();
        for (const auto &entry : entries) {
            mThreadPool.enqueue([moduleBase, moduleSize, &entry, this]() {
                uintptr_t foundAddress =
                    memory::scan::scanSignature(moduleBase, moduleSize, entry.mSig.c_str(), entry.mSig.length());

                if (foundAddress != 0) {
                    uintptr_t        finalAddress = applyOperations(foundAddress, entry.mOperations);
                    std::unique_lock lock{mSymbolMapLock};
                    if (entry.mType == sapphire::codegen::SigDatabase::SigEntry::Type::Data)
                        mResolvedDataSymbols[entry.mSymbol] = finalAddress;
                    else
                        mResolvedFunctionSymbols[entry.mSymbol] = finalAddress;
                    if (entry.hasExtraSymbol())
                        mResolvedFunctionSymbols[entry.mExtraSymbol] = finalAddress;
                }
            });
        }
        mThreadPool.wait_all_finished();
        mThreadPool.stop();
    }

} // namespace sapphire::bootloader