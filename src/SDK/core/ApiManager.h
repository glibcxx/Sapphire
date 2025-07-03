#pragma once

#include "macros/Macros.h"
#include "util/threading/ThreadPool.h"
#include "util/StringLiteral.hpp"
#include "util/Memory.hpp"
#include "util/ApiUniqueId.hpp"
#include "MainModuleInfo.h"

template <util::StringLiteral A>
consteval auto operator""_sig() {
    return A;
}

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

        template <auto Api, auto Callback = nullptr, size_t N>
        uintptr_t scanAndRegisterApi(const char (&sig)[N], bool storeToMap = true) {
            uintptr_t origin = this->_scanApi(sig, N - 1);
            if (!origin) {
                constexpr std::string_view msg = __FUNCSIG__;
                Logger::Error("[core] Failed to find api: {}", msg);
                return 0;
            }
            if constexpr (std::is_invocable_r_v<uintptr_t, decltype(Callback), uintptr_t>)
                origin = Callback(origin);
            if (storeToMap)
                this->_addApiToMap(util::ApiUniqueId::make<Api>(), origin);
            return origin;
        }

        template <util::StringLiteral Sig, typename ApiType, auto Api, auto Callback = nullptr>
        void scanAndRegisterApiAsync(ApiType &result, bool storeToMap = true) {
            this->_submitAsyncScanTask([this, storeToMap, &result] {
                result = memory::toMemberFunc<ApiType>(this->scanAndRegisterApi<Api, Callback>(Sig.value, storeToMap));
            });
        }
    };

    template <util::StringLiteral Sig, auto Api, auto Callback = nullptr>
        requires(Callback == nullptr || std::is_invocable_r_v<uintptr_t, decltype(Callback), uintptr_t>)
    class ApiLoader {
        using ApiType = decltype(Api);

    public:
        // inline static ApiType origin = memory::toMemberFunc<ApiType>(
        //     ApiManager::getInstance().scanAndRegisterApi<Api, Callback>(Sig.value)
        // );
        inline static ApiType origin;

    private:
        inline static int async = (ApiManager::getInstance().scanAndRegisterApiAsync<Sig, ApiType, Api, Callback>(origin), 0);
    };

    constexpr auto deRefLea = [](uintptr_t addr) { return memory::deRef(addr, memory::AsmOperation::LEA); };
    constexpr auto deRefCall = [](uintptr_t addr) { return memory::deRef(addr, memory::AsmOperation::CALL); };

    template <util::StringLiteral Sig, auto Api, auto Callback = deRefLea>
        requires(Callback == nullptr || std::is_invocable_r_v<uintptr_t, decltype(Callback), uintptr_t>)
    uintptr_t loadStatic() {
        return ApiManager::getInstance().scanAndRegisterApi<Api, Callback>(Sig.value, false);
    };

    template <util::StringLiteral Sig, auto Api, auto Callback = deRefLea>
        requires(Callback == nullptr || std::is_invocable_r_v<uintptr_t, decltype(Callback), uintptr_t>)
    void *const *loadVftable() {
        return reinterpret_cast<void *const *>(loadStatic<Sig, Api, Callback>());
    };

} // namespace sapphire
