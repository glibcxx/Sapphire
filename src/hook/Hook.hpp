#pragma once

#include <MinHook.h>

#include "SDK/core/Core.h"
#include "util/TypeTraits.hpp"

namespace hook {
    using namespace type_traits;

    inline bool init() {
        return MH_Initialize() == MH_OK;
    }

    inline void uninit() {
        MH_Uninitialize();
    }

    template <typename T>
    constexpr uintptr_t funcToAddr(T fn) {
        if constexpr (std::is_member_function_pointer_v<T>)
            return std::bit_cast<uintptr_t>(fn);
        else
            return reinterpret_cast<uintptr_t>(fn);
    }

    /**
     * @brief Hook一个函数
     * @param targetAddr 目标函数指针，不可为空
     * @param detourAddr hook函数指针
     * @return 指向原始目标函数的跳板，如果hook失败，返回`nullptr`
     *
     * @details Hook后，通过`targetAddr`进行的调用都会跳转至`detourAddr`，想要调用原始函数，需要通过返回值调用
     */
    template <typename Target, typename Detour>
        requires(is_func_ptr<Target> && is_func_ptr<Detour>)
    Target hookFunc(Target targetAddr, Detour detourAddr) {
        if (!targetAddr) return nullptr;
        LPVOID trampoline = nullptr;
        if (MH_CreateHook((LPVOID)funcToAddr(targetAddr), (LPVOID)funcToAddr(detourAddr), &trampoline) != MH_OK) return nullptr;
        if (MH_EnableHook((LPVOID)funcToAddr(targetAddr)) != MH_OK) return nullptr;
        return std::bit_cast<Target>(trampoline);
    }

    template <typename Target>
        requires(is_func_ptr<Target>)
    bool unhookFunc(Target targetAddr) {
        return targetAddr && MH_DisableHook((LPVOID)funcToAddr(targetAddr)) == MH_OK;
    }

    class DummyClass {};
} // namespace hook

#define IMPL_HOOK_TYPE(Static, Const, FPtr, Call, HookName, TypeName, targetAddr, RetType, ...)                 \
    class HookName : public TypeName {                                                                          \
        using FuncPtrType = RetType FPtr(__VA_ARGS__) Const;                                                    \
        inline static FuncPtrType   sdkOriginal = nullptr;                                                      \
        inline static FuncPtrType   trampoline = nullptr;                                                       \
                                                                                                                \
        template <typename... Args>                                                                             \
        Static RetType origin(Args &&...args) Const {                                                           \
            return Call(std::forward<Args>(args)...);                                                           \
        }                                                                                                       \
        Static RetType detour(__VA_ARGS__) Const;                                                               \
                                                                                                                \
    public:                                                                                                     \
        static bool hook() {                                                                                    \
            if (sdkOriginal = core::getOrigin<FuncPtrType>(                                                     \
                    util::ApiUniqueId::make<(FuncPtrType) & targetAddr>()                                       \
                ))                                                                                              \
                return trampoline = hook::hookFunc(sdkOriginal, &HookName::detour);                             \
            Logger::ErrorBox(L"[HookError] Target addr [" #targetAddr "] not found! The SDK might be broken."); \
            return false;                                                                                       \
        }                                                                                                       \
                                                                                                                \
        static void unhook() {                                                                                  \
            hook::unhookFunc(sdkOriginal);                                                                      \
        }                                                                                                       \
    };                                                                                                          \
    inline RetType HookName::detour(__VA_ARGS__) Const

// Hook SDK内的函数，用下面3个

#define HOOK_STATIC(HookName, targetAddr, RetType, ...) \
    IMPL_HOOK_TYPE(static, , (*), trampoline, HookName, hook::DummyClass, targetAddr, RetType, __VA_ARGS__)

#define HOOK_TYPE(HookName, TypeName, targetAddr, RetType, ...) \
    IMPL_HOOK_TYPE(, , (TypeName::*), (this->*trampoline), HookName, TypeName, targetAddr, RetType, __VA_ARGS__)

#define HOOK_TYPE_CONST(HookName, TypeName, targetAddr, RetType, ...) \
    IMPL_HOOK_TYPE(, const, (TypeName::*), (this->*trampoline), HookName, TypeName, targetAddr, RetType, __VA_ARGS__)

#define IMPL_HOOK_RAW_TYPE(Static, Const, FPtr, Call, HookName, TypeName, targetAddr, RetType, ...) \
    class HookName : public TypeName {                                                              \
        using FuncPtrType = RetType FPtr(__VA_ARGS__) Const;                                        \
        inline static FuncPtrType   trampoline = nullptr;                                           \
                                                                                                    \
        template <typename... Args>                                                                 \
        Static RetType origin(Args &&...args) Const {                                               \
            return Call(std::forward<Args>(args)...);                                               \
        }                                                                                           \
        Static RetType detour(__VA_ARGS__) Const;                                                   \
                                                                                                    \
    public:                                                                                         \
        static bool hook() {                                                                        \
            return trampoline = hook::hookFunc((FuncPtrType) & targetAddr, &HookName::detour);      \
        }                                                                                           \
                                                                                                    \
        static void unhook() {                                                                      \
            hook::unhookFunc((FuncPtrType) & targetAddr);                                           \
        }                                                                                           \
    };                                                                                              \
    inline RetType HookName::detour(__VA_ARGS__) Const

// Hook 非 SDK 内的函数，用下面3个

#define HOOK_RAW_STATIC(HookName, targetAddr, RetType, ...) \
    IMPL_HOOK_RAW_TYPE(static, , (*), trampoline, HookName, hook::DummyClass, targetAddr, RetType, __VA_ARGS__)

#define HOOK_RAW_TYPE(HookName, TypeName, targetAddr, RetType, ...) \
    IMPL_HOOK_RAW_TYPE(, , (TypeName::*), (this->*trampoline), HookName, TypeName, targetAddr, RetType, __VA_ARGS__)

#define HOOK_RAW_TYPE_CONST(HookName, TypeName, targetAddr, RetType, ...) \
    IMPL_HOOK_RAW_TYPE(, const, (TypeName::*), (this->*trampoline), HookName, TypeName, targetAddr, RetType, __VA_ARGS__)