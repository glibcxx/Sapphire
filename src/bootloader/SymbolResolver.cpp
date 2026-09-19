/*
    SymbolResolver

    SymbolResolver会读取特征码数据库，扫描所有特征码。
    IatPatcher会使用扫描后的结果完成修补。
*/

#include "SymbolResolver.h"
#include <Windows.h>
#include <Psapi.h>
#include <cassert>
#include <fstream>
#include <filesystem>

#include "common/DecoratedName.hpp"
#include "common/Memory.hpp"
#include "common/MemoryScanning.hpp"
#include "common/ScopedTimer.hpp"
#include "common/coroutine/Coroutine.hpp"
#include "common/coroutine/AsyncScope.hpp"
#include "common/IPC/Client.h"

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
                currentAddress += op.data.disp;
                break;
            case codegen::SigDatabase::SigOpType::Deref:
                currentAddress = *reinterpret_cast<uintptr_t *>(currentAddress);
                break;
            case codegen::SigDatabase::SigOpType::Call:
                currentAddress = sapphire::ripRel(currentAddress, sapphire::InstType::CALL);
                break;
            case codegen::SigDatabase::SigOpType::Mov:
                currentAddress = sapphire::ripRel(currentAddress, sapphire::InstType::MOV);
                break;
            case codegen::SigDatabase::SigOpType::Lea:
                currentAddress = sapphire::ripRel(currentAddress, sapphire::InstType::LEA);
                break;
            case codegen::SigDatabase::SigOpType::RipRel:
                currentAddress = sapphire::ripRel(currentAddress, op.data.ripRel.offset, op.data.ripRel.insLen);
                break;
            case codegen::SigDatabase::SigOpType::Deref32:
                currentAddress = *reinterpret_cast<uint32_t *>(currentAddress);
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

    void SymbolResolver::resolve(ipc::Client &log) {
        log.send(ipc::status::Success, "[Bootloader] Resolving symbols, please wait...");
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
        mResolvedFunctionSymbols[sapphire::abi::Decorator<&SymbolResolver::get>::value.value] =
            (uintptr_t)&SymbolResolver::get;

        struct ScanResult {
            const codegen::SigDatabase::SigEntry *entry;
            uintptr_t                             address;
        };

        std::atomic<size_t> completedTasks = 0;

        auto progressTask = [&log, &completedTasks, totalTasks = entries.size()](
                                coro::StaticThreadPool &pool, coro::IoContext &ioCtx
                            ) -> coro::Task<void> {
            using namespace std::chrono_literals;
            size_t lastProgress = 0;
            while (true) {
                const size_t completed = completedTasks.load(std::memory_order_relaxed);
                if (completed >= totalTasks) {
                    log.send(ipc::status::Success, "[Bootloader] Scanning 100% complete.");
                    break;
                }

                size_t currentProgress = (completed * 100) / totalTasks;
                if (currentProgress > lastProgress) {
                    lastProgress = currentProgress;
                    log.send(
                        ipc::status::Success,
                        std::format("[Bootloader] Scanning... {}%", currentProgress)
                    );
                }
                co_await ioCtx.scheduleAfter(25ms);
            }
            ioCtx.stop();
        };

        coro::StaticThreadPool pool{std::thread::hardware_concurrency()};

        auto scanTask = [&completedTasks, moduleBase, moduleSize, this](
                            coro::StaticThreadPool               &pool,
                            const codegen::SigDatabase::SigEntry &entry,
                            ScanResult                           &result
                        ) -> coro::Task<void> {
            co_await pool.schedule();
            uintptr_t foundAddress =
                sapphire::scanSignature(moduleBase, moduleSize, entry.mSig.c_str(), entry.mSig.length());
            completedTasks.fetch_add(1, std::memory_order_relaxed);
            result = ScanResult{&entry, foundAddress ? applyOperations(foundAddress, entry.mOperations) : 0};
        };

        auto mainTask = [&](coro::StaticThreadPool &pool, coro::IoContext &ioCtx) -> coro::Task<> {
            coro::AsyncScope scope;

            std::vector<ScanResult> results;
            results.resize(entries.size());
            for (size_t idx = 0; const auto &entry : entries) {
                scope.spawn(scanTask(pool, entry, results[idx]));
                ++idx;
            }

            co_await scope.join();

            for (auto &&result : results) {
                if (result.address != 0) {
                    if (result.entry->mType == sapphire::codegen::SigDatabase::SigEntry::Type::Data)
                        mResolvedDataSymbols[result.entry->mSymbol] = result.address;
                    else
                        mResolvedFunctionSymbols[result.entry->mSymbol] = result.address;
                    if (result.entry->hasExtraSymbol())
                        mResolvedFunctionSymbols[result.entry->mExtraSymbol] = result.address;
                }
            }
        };

        coro::IoContext ioContext;

        static sapphire::TimerToken token;
        {
            sapphire::ScopedTimer timer{token};
            syncWait(
                whenAll(
                    progressTask(pool, ioContext),
                    mainTask(pool, ioContext),
                    [](coro::IoContext &ioCtx) -> coro::Task<> {
                        ioCtx.processEvents();
                        co_return;
                    }(ioContext)
                )
            );
        }

        log.send(
            ipc::status::Success,
            std::format(
                "[Bootloader] Resolving symbols. Done. ({})",
                std::chrono::duration_cast<std::chrono::milliseconds>(token.getDuration())
            )
        );
    }

} // namespace sapphire::bootloader