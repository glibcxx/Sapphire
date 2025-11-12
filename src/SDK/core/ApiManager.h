#pragma once

#include "macros/Macros.h"
#include <source_location>
#include "util/threading/ThreadPool.h"
#include "util/Memory.hpp"
#include "util/ApiUniqueId.hpp"
#include "util/TypeTraits.hpp"
#include "MainModuleInfo.h"
#include "Signature.h"

#include "SDK/api/sapphire/hook/SafeHook.h"

namespace sapphire {

    class ApiManager {
        MainModuleInfo   mMainModuleInfo;
        util::ThreadPool mThreadPool;

        using ApiMap = std::unordered_map<std::string_view, uintptr_t>;

        ApiMap mFunctionApiMap;
        ApiMap mDataApiMap;

        mutable std::mutex mLock;

        SPHR_API uintptr_t _scanApi(const char *sig, size_t sigLength);

        void _submitAsyncScanTask(std::function<void()> &&task) {
            this->mThreadPool.enqueue(std::move(task));
        }

    public:
        SPHR_API static ApiManager &getInstance();

        const ApiMap &getFunctionApiMap() const {
            return this->mFunctionApiMap;
        }
        const ApiMap &getDataApiMap() const {
            return this->mDataApiMap;
        }

        bool addDecoratedName(std::string_view dName, uintptr_t addr, bool functionApi) {
            bool success = false;
            {
                std::unique_lock lock{mLock};
                if (functionApi)
                    success = this->mFunctionApiMap.try_emplace(dName, addr).second;
                else
                    success = this->mDataApiMap.try_emplace(dName, addr).second;
            }
#ifdef SPHR_DEBUG
            if (!success)
                Logger::Warn("[ApiManager] Api with name {} already exists", dName);
#endif
            return success;
        }

        uintptr_t findTarget(std::string_view dName) {
            std::unique_lock lock{mLock};
            auto             it = this->mFunctionApiMap.find(dName);
            if (it == this->mFunctionApiMap.end())
                return 0;
            else
                return it->second;
        }

        template <auto ptr>
        uintptr_t findTarget() {
            return findTarget(util::Decorator<ptr, true, true>::value.view());
        }

        void startThreadPool() {
            this->mThreadPool.start();
        }

        void stopThreadPool() {
            this->mThreadPool.stop();
        }

        void waitAllFinished() {
            this->mThreadPool.wait_all_finished();
        }

        template <signature::Signature Sig>
        uintptr_t scanAndRegisterApi(std::string_view dName, bool functionApi) {
            uintptr_t origin = this->_scanApi(Sig.mSig.value, Sig.mSig.size - 1);
            if (!origin) {
                Logger::Error("[ApiManager] Failed to find api: {}", dName);
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

    template <signature::Signature Sig, auto Api, auto = nullptr>
    class ApiLoader;

    template <signature::Signature Sig, auto Api>
    class ApiLoader<Sig, Api, nullptr> {
        using ApiType = decltype(Api);
        using Decorated = util::Decorator<Api, true, true>;
        static_assert(
            !Decorated::value.view().starts_with("??_9"),
            "Please use ApiLoader<..., ..., SPHR_FUNCDNAME> for virtual function"
        );

    public:
        inline static ApiType origin;

    private:
        inline static int async = ApiManager::getInstance().scanAndRegisterApiAsync<Sig>(
            Decorated::value.view(), origin, true
        );
    };

    template <signature::Signature Sig, auto Api, util::StringLiteral RawDecoratedName>
    class ApiLoader<Sig, Api, RawDecoratedName> {
        using ApiType = decltype(Api);
        using Decorated = util::Decorator<Api, true, true>;
        static_assert(
            Decorated::value.view().starts_with("??_9"),
            "Implicit template param 'RawDecoratedName' can only be specified by virtual fucntion"
        );

    public:
        inline static ApiType origin;

    private:
        inline static int async = ApiManager::getInstance().scanAndRegisterApiAsync<Sig>(
            Decorated::value.view(), RawDecoratedName.view(), origin, true
        );
    };

    constexpr auto deRefLea = [](uintptr_t addr) { return memory::deRef(addr, memory::AsmOperation::LEA); };
    constexpr auto deRefCall = [](uintptr_t addr) { return memory::deRef(addr, memory::AsmOperation::CALL); };
    constexpr auto deRefMov = [](uintptr_t addr) { return memory::deRef(addr, memory::AsmOperation::MOV); };

    template <signature::Signature Sig, auto Api, typename DataType>
    DataType &loadStatic() {
        if constexpr (std::is_reference_v<DataType>)
            return **std::bit_cast<std::remove_reference_t<DataType> **>(
                ApiManager::getInstance().scanAndRegisterApi<Sig, Api>(util::Decorator<Api>::value.view(), false)
            );
        else
            return *std::bit_cast<DataType *>(ApiManager::getInstance().scanAndRegisterApi<Sig>(
                util::Decorator<Api>::value.view(), false
            ));
    };

    template <signature::Signature Sig, auto Api>
    void *const *loadVftable() {
        return &loadStatic<deRefLea | Sig, Api, void *const>();
    };

} // namespace sapphire
