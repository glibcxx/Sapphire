#include "SymbolResolver.h"
#include <Windows.h>
#include <cassert>
#include <fstream>
#include <filesystem>
#include <Psapi.h>

#include "common/DecoratedName.hpp"
#include "common/MemoryScanning.hpp"
#include "common/ScopedTimer.hpp"
#include "common/coroutine/Coroutine.hpp"

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
                            ) -> coro::Task<> {
            using namespace std::chrono_literals;
            co_await pool.schedule();

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

                co_await ioCtx.scheduleAfter(50ms);
                co_await pool.schedule(); // resume on thread pool
            }
            ioCtx.stop();
        };

        coro::StaticThreadPool pool;

        auto scanTask = [&completedTasks, moduleBase, moduleSize, this](
                            coro::StaticThreadPool               &pool,
                            const codegen::SigDatabase::SigEntry &entry
                        ) -> coro::Task<ScanResult> {
            co_await pool.schedule();
            uintptr_t foundAddress =
                sapphire::scanSignature(moduleBase, moduleSize, entry.mSig.c_str(), entry.mSig.length());
            completedTasks.fetch_add(1, std::memory_order_relaxed);
            co_return ScanResult{&entry, foundAddress ? applyOperations(foundAddress, entry.mOperations) : 0};
        };

        auto mainTask = [&](coro::StaticThreadPool &pool, coro::IoContext &ioCtx) -> coro::Task<> {
            co_await pool.schedule();
            std::vector<coro::Task<ScanResult>> tasks;
            tasks.reserve(entries.size());
            for (const auto &entry : entries) {
                tasks.emplace_back(scanTask(pool, entry));
            }
            auto results = co_await whenAll(std::move(tasks));

            for (auto &&res : results) {
                auto result = res.result();
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