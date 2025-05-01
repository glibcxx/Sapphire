#pragma once

#include "Version.h"
#include "hook/Hook.hpp"
#include "logger/Logger.hpp"

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

    SDK_API uintptr_t registryApi(const char *sig, size_t sigLength, uintptr_t api);

    template <auto Api, typename T = uintptr_t, size_t N>
    auto registryApi(const char (&sig)[N], bool storeToMap = true) {
        auto a = std::bit_cast<T>(registryApi(sig, N - 1, storeToMap ? std::bit_cast<uintptr_t>(Api) : 0));
        if (!a) {
            constexpr std::wstring_view msg = &CONCAT(L, __FUNCSIG__)[31];
            Logger::ErrorBox(L"registryApi failed:\n {}", msg.substr(0, msg.find_last_of(L",0") - 3)); // 没扫到函数？弹个Error
        }
        return a;
    }

    // 不知道为啥，反正我就要把signature塞进模板参数

    template <StringLiteral Sig, auto Api, auto Callback = nullptr>
        requires(Callback == nullptr || std::is_invocable_r_v<decltype(Api), decltype(Callback), decltype(Api)>)
    struct ApiLoader {
        using ApiType = decltype(Api);
        inline static ApiType origin = []() -> ApiType {
            ApiType origin = registryApi<Api, ApiType>(Sig.value);
            if constexpr (std::is_invocable_r_v<decltype(Api), decltype(Callback), decltype(Api)>)
                return Callback(origin);
            else
                return origin;
        }();
    };

    template <StringLiteral Sig, auto Api, auto Callback = nullptr>
        requires(Callback == nullptr || std::is_invocable_r_v<decltype(Callback), uintptr_t>)
    void *const *loadVftable() {
        uintptr_t addr = registryApi<Api, uintptr_t>(Sig.value, false);
        if constexpr (std::is_invocable_r_v<uintptr_t, decltype(Callback), uintptr_t>)
            return reinterpret_cast<void *const *>(Callback(addr));
        else
            return reinterpret_cast<void *const *>(addr);
    };

} // namespace core
