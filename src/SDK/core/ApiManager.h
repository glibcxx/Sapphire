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

        std::unordered_map<util::ApiUniqueId, uintptr_t> mSdkApi2TargetAddr;

        void _addApiToMap(util::ApiUniqueId api, uintptr_t originalAdr) {
            this->mSdkApi2TargetAddr.emplace(api, originalAdr);
        }

        SDK_API uintptr_t _scanApi(const char *sig, size_t sigLength);

        void _submitAsyncScanTask(std::function<void()> &&task) {
            this->mThreadPool.enqueue(std::move(task));
        }

    public:
        SDK_API static ApiManager &getInstance();

        uintptr_t findTarget(util::ApiUniqueId api) const noexcept {
            auto it = this->mSdkApi2TargetAddr.find(api);
            if (it == this->mSdkApi2TargetAddr.end())
                return 0;
            else
                return it->second;
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

        template <auto Api>
        static void assertApi() {
            constexpr std::string_view sig = &__FUNCSIG__[45];
            constexpr std::string_view msg = sig.substr(0, sig.size() - 7);
            Logger::Error("[ApiManager] Failed to find api: {}", msg);
        }

        template <auto Api>
        static void assertApi(std::source_location loc) {
            constexpr std::string_view sig = &__FUNCSIG__[45];
            constexpr std::string_view msg = sig.substr(0, sig.size() - 30);
            Logger::Error("[ApiManager] Failed to find data: {} at [{}:{}:{}]", msg, loc.file_name(), loc.line(), loc.column());
        }

        template <signature::Signature Sig, auto Api>
        uintptr_t scanAndRegisterApi(bool storeToMap = true) {
            uintptr_t origin = this->_scanApi(Sig.mSig.value, Sig.mSig.size - 1);
            if (!origin) {
                assertApi<Api>();
                return 0;
            }
            origin = Sig(origin);
            if (storeToMap)
                this->_addApiToMap(util::ApiUniqueId::make<Api>(), origin);
            return origin;
        }

        template <signature::Signature Sig, auto Api>
        uintptr_t scanAndRegisterApi(std::source_location loc, bool storeToMap = true) {
            uintptr_t origin = this->_scanApi(Sig.mSig.value, Sig.mSig.size - 1);
            if (!origin) {
                assertApi<Api>(loc);
                return 0;
            }
            origin = Sig(origin);
            if (storeToMap)
                this->_addApiToMap(util::ApiUniqueId::make<Api>(), origin);
            return origin;
        }

        template <signature::Signature Sig, typename ApiType, auto Api>
        void scanAndRegisterApiAsync(ApiType &result, bool storeToMap = true) {
            this->_submitAsyncScanTask([this, storeToMap, &result] {
                result = memory::toMemberFunc<ApiType>(this->scanAndRegisterApi<Sig, Api>(storeToMap));
            });
        }
    };

    template <signature::Signature Sig, auto Api>
    class ApiLoader {
        using ApiType = decltype(Api);

    public:
        inline static ApiType origin;

    private:
        inline static int async = (ApiManager::getInstance().scanAndRegisterApiAsync<Sig, ApiType, Api>(origin), 0);
    };

    constexpr auto deRefLea = [](uintptr_t addr) { return memory::deRef(addr, memory::AsmOperation::LEA); };
    constexpr auto deRefCall = [](uintptr_t addr) { return memory::deRef(addr, memory::AsmOperation::CALL); };
    constexpr auto deRefMov = [](uintptr_t addr) { return memory::deRef(addr, memory::AsmOperation::MOV); };

    template <signature::Signature Sig, auto Api, typename DataType>
    DataType &loadStatic(std::source_location loc = std::source_location::current()) {
        if constexpr (std::is_reference_v<DataType>)
            return **std::bit_cast<std::remove_reference_t<DataType> **>(ApiManager::getInstance().scanAndRegisterApi<Sig, Api>(loc, false));
        else
            return *std::bit_cast<DataType *>(ApiManager::getInstance().scanAndRegisterApi<Sig, Api>(loc, false));
    };

    template <signature::Signature Sig, auto Api>
    void *const *loadVftable(std::source_location loc = std::source_location::current()) {
        return loadStatic<Sig, Api, void *const *>(loc);
    };

} // namespace sapphire
