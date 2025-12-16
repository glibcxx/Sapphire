#pragma once

#include "macros/Macros.h"
#include <source_location>
#include "common/threading/ThreadPool.h"
#include "common/Memory.hpp"
#include "common/ApiUniqueId.hpp"
#include "common/TypeTraits.hpp"
#include "MainModuleInfo.h"
#include "common/Signature.h"

#include "bootloader/SymbolResolver.h"

#include "SDK/api/sapphire/hook/SafeHook.h"

namespace sapphire {

    class SymbolResolver {
        MainModuleInfo   mMainModuleInfo;
        util::ThreadPool mThreadPool;

        using ApiMap = std::unordered_map<std::string, uintptr_t>;

        bootloader::SymbolResolver &mBLSymbolResolver;

        mutable std::mutex mLock;

        SPHR_API uintptr_t _scanApi(const char *sig, size_t sigLength);

        void _submitAsyncScanTask(std::function<void()> &&task) {
            this->mThreadPool.enqueue(std::move(task));
        }

        SymbolResolver(bootloader::SymbolResolver &blsr);

    public:
        SPHR_API static SymbolResolver &getInstance();

        const ApiMap &getFunctionApiMap() const {
            return mBLSymbolResolver.getResolvedFunctionSymbols();
        }
        const ApiMap &getDataApiMap() const {
            return mBLSymbolResolver.getResolvedDataSymbols();
        }

        bool addDecoratedName(std::string_view dName, uintptr_t addr, bool functionApi) {
            bool success = false;
            {
                std::unique_lock lock{mLock};
                if (functionApi)
                    success = mBLSymbolResolver.getResolvedFunctionSymbols().try_emplace(std::string{dName}, addr).second;
                else
                    success = mBLSymbolResolver.getResolvedDataSymbols().try_emplace(std::string{dName}, addr).second;
            }
#ifdef SPHR_DEBUG
            if (!success)
                sapphire::warn("SymbolResolver: Api with name {} already exists", dName);
#endif
            return success;
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

        void startResolvingThreads() {
            this->mThreadPool.start();
        }

        void stopResolvingThreads() {
            this->mThreadPool.stop();
        }

        void resolveAll() {
            this->mThreadPool.wait_all_finished();
        }

        template <signature::Signature Sig>
        uintptr_t scanAndRegisterApi(std::string_view dName, bool functionApi) {
            uintptr_t origin = this->_scanApi(Sig.mSig.value, Sig.mSig.size - 1);
            if (!origin) {
                sapphire::error("SymbolResolver: Failed to find api: {}", dName);
                return 0;
            }
            origin = Sig(origin);
            this->addDecoratedName(dName, origin, functionApi);
            return origin;
        }

        template <signature::Signature Sig, typename ApiType>
        int scanAndRegisterApiAsync(std::string_view dName, ApiType &result, bool functionApi) {
            this->_submitAsyncScanTask([this, dName, &result, functionApi] {
                result = memory::toMemberFunc<ApiType>(scanAndRegisterApi<Sig>(dName, functionApi));
            });
            return 0;
        }

        template <signature::Signature Sig, typename ApiType>
        int scanAndRegisterApiAsync(
            std::string_view dName, std::string_view dName2, ApiType &result, bool functionApi
        ) {
            this->_submitAsyncScanTask([this, dName, dName2, &result, functionApi] {
                auto addr = scanAndRegisterApi<Sig>(dName, functionApi);
                this->addDecoratedName(dName2, addr, functionApi);
                result = memory::toMemberFunc<ApiType>(addr);
            });
            return 0;
        }
    };

    constexpr auto deRefLea = [](uintptr_t addr) { return memory::deRef(addr, memory::AsmOperation::LEA); };
    constexpr auto deRefCall = [](uintptr_t addr) { return memory::deRef(addr, memory::AsmOperation::CALL); };
    constexpr auto deRefMov = [](uintptr_t addr) { return memory::deRef(addr, memory::AsmOperation::MOV); };

    namespace bind {

        template <signature::Signature Sig, auto Api, auto = nullptr>
        class Fn;

        template <signature::Signature Sig, auto Api>
        class Fn<Sig, Api, nullptr> {
            using ApiType = decltype(Api);
            using Decorated = util::Decorator<Api, true, true>;
            static_assert(
                !Decorated::value.view().starts_with("??_9"),
                "Please use sapphire::bind::Fn<..., ..., SPHR_FUNCDNAME> for virtual function"
            );

        public:
            inline static ApiType origin;

        private:
            inline static int async = SymbolResolver::getInstance().scanAndRegisterApiAsync<Sig>(
                Decorated::value.view(), origin, true
            );
        };

        template <signature::Signature Sig, auto Api, util::StringLiteral RawDecoratedName>
        class Fn<Sig, Api, RawDecoratedName> {
            using ApiType = decltype(Api);
            using Decorated = util::Decorator<Api, true, true>;
            static_assert(
                Decorated::value.view().starts_with("??_9"),
                "Implicit template param 'RawDecoratedName' can only be specified by virtual fucntion"
            );

        public:
            inline static ApiType origin;

        private:
            inline static int async = SymbolResolver::getInstance().scanAndRegisterApiAsync<Sig>(
                Decorated::value.view(), RawDecoratedName.view(), origin, true
            );
        };

        template <signature::Signature Sig, auto Api, typename DataType>
        DataType &data() {
            if constexpr (std::is_reference_v<DataType>)
                return **std::bit_cast<std::remove_reference_t<DataType> **>(
                    SymbolResolver::getInstance().scanAndRegisterApi<Sig, Api>(util::Decorator<Api>::value.view(), false)
                );
            else
                return *std::bit_cast<DataType *>(SymbolResolver::getInstance().scanAndRegisterApi<Sig>(
                    util::Decorator<Api>::value.view(), false
                ));
        };

        template <signature::Signature Sig, auto Api>
        void *const *vftbl() {
            return &data<deRefLea | Sig, Api, void *const>();
        };

    } // namespace bind

} // namespace sapphire
