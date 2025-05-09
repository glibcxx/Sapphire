#pragma once

#include "Version.h"
#include "hook/Hook.hpp"
#include "logger/LogBox.hpp"

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
    SDK_API uintptr_t getImagebase();

    SDK_API uintptr_t getOrigin(uintptr_t api);

    template <typename T, typename U>
        requires(!std::is_same_v<T, uintptr_t>)
    T getOrigin(U api) {
        return std::bit_cast<T>(getOrigin(std::bit_cast<uintptr_t>(api)));
    }

    SDK_API uintptr_t scanApi(const char *sig, size_t sigLength);

    SDK_API void addToMap(uintptr_t api, uintptr_t origin);

    template <typename Ret = uintptr_t, size_t N>
    Ret scanApi(const char (&sig)[N]) {
        return std::bit_cast<Ret>(scanApi(sig, N - 1));
    }

    template <auto Api, auto Callback = nullptr, size_t N>
    uintptr_t registryApi(const char (&sig)[N], bool storeToMap = true) {
        uintptr_t origin = scanApi(sig, N - 1);
        if (!origin) {
            constexpr std::wstring_view msg = &CONCAT(L, __FUNCSIG__)[44];
            Logger::ErrorBox(L"registryApi failed:\n {}", msg.substr(0, msg.rfind(L",0x"))); // 没扫到函数？弹个Error
            return 0;
        }
        if constexpr (std::is_invocable_r_v<uintptr_t, decltype(Callback), uintptr_t>)
            origin = Callback(origin);
        if (storeToMap)
            addToMap(std::bit_cast<uintptr_t>(Api), origin);
        return origin;
    }

    template <StringLiteral Sig, auto Api, auto Callback = nullptr>
        requires(Callback == nullptr || std::is_invocable_r_v<uintptr_t, decltype(Callback), uintptr_t>)
    struct ApiLoader {
        using ApiType = decltype(Api);
        inline static ApiType origin = std::bit_cast<ApiType>(registryApi<Api, Callback>(Sig.value));
    };

    template <StringLiteral Sig, auto Api, auto Callback = nullptr>
        requires(Callback == nullptr || std::is_invocable_r_v<uintptr_t, decltype(Callback), uintptr_t>)
    uintptr_t loadStatic() {
        return registryApi<Api, Callback>(Sig.value, false);
    };

    template <StringLiteral Sig, auto Api, auto Callback = nullptr>
        requires(Callback == nullptr || std::is_invocable_r_v<uintptr_t, decltype(Callback), uintptr_t>)
    void *const *loadVftable() {
        return reinterpret_cast<void *const *>(loadStatic<Sig, Api, Callback>());
    };

} // namespace core
