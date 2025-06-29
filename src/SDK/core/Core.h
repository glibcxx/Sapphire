#pragma once

#include <string>
#include "Version.h"
#include "util/StringLiteral.hpp"
#include "util/Memory.hpp"
#include "src/SDK/api/sapphire/hook/Hook.h"

#include <Windows.h>
#include <Psapi.h>

namespace moduleInfo {
    SDK_API extern HWND     gMainWindow;
    SDK_API extern uint64_t gStartTime;

} // namespace moduleInfo

template <std::size_t N>
struct StringLiteral {
    char value[N]{};
    consteval StringLiteral(const char (&str)[N]) { std::copy_n(str, N, value); };
};

template <StringLiteral A>
consteval auto operator""_sig() {
    return A;
}

namespace core {
    class SapphireModuleInfo {
    public:
        HMODULE               sapphireModuleHandle = nullptr;
        MODULEINFO            sapphireModuleInfo;
        std::filesystem::path sapphireHome;

        SapphireModuleInfo();
    };

    SDK_API uintptr_t getImagebase();

    SDK_API SapphireModuleInfo &getSapphireInfo();

    SDK_API uintptr_t getOrigin(util::ApiUniqueId api);

    template <typename T>
    T getOrigin(util::ApiUniqueId api) {
        return std::bit_cast<T>(getOrigin(api));
    }

    SDK_API uintptr_t scanApi(const char *sig, size_t sigLength);

    template <typename Ret = uintptr_t, size_t N>
    Ret scanApi(const char (&sig)[N]) {
        return std::bit_cast<Ret>(scanApi(sig, N - 1));
    }

    template <auto Api, auto Callback = nullptr, size_t N>
    uintptr_t registryApi(const char (&sig)[N], bool storeToMap = true) {
        uintptr_t origin = scanApi(sig, N - 1);
        if (!origin) {
            constexpr std::string_view msg = &__FUNCSIG__[43];
            Logger::Error("registryApi failed: {}", msg.substr(0, msg.rfind(",0x"))); // 没扫到函数？弹个Error
            return 0;
        }
        if constexpr (std::is_invocable_r_v<uintptr_t, decltype(Callback), uintptr_t>)
            origin = Callback(origin);
        if (storeToMap)
            hook::HookManager::getInstance().resitryApi(util::ApiUniqueId::make<Api>(), origin);
        return origin;
    }

    template <StringLiteral Sig, auto Api, auto Callback = nullptr>
        requires(Callback == nullptr || std::is_invocable_r_v<uintptr_t, decltype(Callback), uintptr_t>)
    class ApiLoader {
        using ApiType = decltype(Api);

    public:
        inline static ApiType origin = memory::toMemberFunc<ApiType>(registryApi<Api, Callback>(Sig.value));
    };

    constexpr auto deRefLea = [](uintptr_t addr) { return memory::deRef(addr, memory::AsmOperation::LEA); };
    constexpr auto deRefCall = [](uintptr_t addr) { return memory::deRef(addr, memory::AsmOperation::CALL); };

    template <StringLiteral Sig, auto Api, auto Callback = deRefLea>
        requires(Callback == nullptr || std::is_invocable_r_v<uintptr_t, decltype(Callback), uintptr_t>)
    uintptr_t loadStatic() {
        return registryApi<Api, Callback>(Sig.value, false);
    };

    template <StringLiteral Sig, auto Api, auto Callback = deRefLea>
        requires(Callback == nullptr || std::is_invocable_r_v<uintptr_t, decltype(Callback), uintptr_t>)
    void *const *loadVftable() {
        return reinterpret_cast<void *const *>(loadStatic<Sig, Api, Callback>());
    };

} // namespace core
