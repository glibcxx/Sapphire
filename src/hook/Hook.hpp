#pragma once

#include <bit>

#include <MinHook.h>

#include "Memory.hpp"
#include "Macros.h"

namespace hook {
    inline bool init() {
        return MH_Initialize() == MH_OK;
    }

    inline void uninit() {
        MH_Uninitialize();
    }

    template <typename T>
    constexpr bool is_func_ptr = std::is_pointer_v<T> && std::is_function_v<std::remove_pointer_t<T>> || std::is_member_function_pointer_v<T>;

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
} // namespace hook

/**
 * @brief 创建一个hook，通过 `HookName::hook()` 激活hook，`HookName::unhook()` 取消hook
 *
 * @param HookName (自定义) Hook名称
 * @param targetAddr 目标函数地址
 * @param RetType 目标函数返回类型
 * @param ... 形参类型与形参名
 */
#define HOOK_STATIC(HookName, targetAddr, RetType, ...)                                           \
    class HookName {                                                                              \
        using FuncPtrType = RetType (*)(__VA_ARGS__);                                             \
        static_assert(hook::is_func_ptr<FuncPtrType>, #targetAddr " should be function pointer"); \
                                                                                                  \
        inline static FuncPtrType sdkOriginal = nullptr;                                          \
        inline static FuncPtrType trampoline = nullptr;                                           \
                                                                                                  \
        template <typename... Args>                                                               \
        static RetType origin(Args &&...args)                                                     \
            requires(std::is_invocable_v<FuncPtrType, Args...>)                                   \
        {                                                                                         \
            return trampoline(std::forward<Args>(args)...);                                       \
        }                                                                                         \
        static RetType detour(__VA_ARGS__);                                                       \
                                                                                                  \
    public:                                                                                       \
        static bool hook() {                                                                      \
            if (sdkOriginal = core::getOrigin<FuncPtrType>(&targetAddr))                          \
                return trampoline = hook::hookFunc(sdkOriginal, &HookName::detour);               \
            LogBox::Error(L"Target addr [" #targetAddr "] not found!");                           \
            return false;                                                                         \
        }                                                                                         \
                                                                                                  \
        static void unhook() {                                                                    \
            hook::unhookFunc(sdkOriginal);                                                        \
        }                                                                                         \
    };                                                                                            \
    inline RetType HookName::detour(__VA_ARGS__)

#define HOOK_TYPE(HookName, TypeName, targetAddr, RetType, ...)                                   \
    class HookName : public TypeName {                                                            \
        using FuncPtrType = RetType (TypeName::*)(__VA_ARGS__);                                   \
        static_assert(hook::is_func_ptr<FuncPtrType>, #targetAddr " should be function pointer"); \
                                                                                                  \
        inline static FuncPtrType sdkOriginal = nullptr;                                          \
        inline static FuncPtrType trampoline = nullptr;                                           \
                                                                                                  \
        template <typename... Args>                                                               \
        RetType origin(Args &&...args)                                                            \
            requires(std::is_invocable_v<FuncPtrType, TypeName, Args...>)                         \
        {                                                                                         \
            return (this->*trampoline)(std::forward<Args>(args)...);                              \
        }                                                                                         \
        RetType detour(__VA_ARGS__);                                                              \
                                                                                                  \
    public:                                                                                       \
        static bool hook() {                                                                      \
            if (sdkOriginal = core::getOrigin<FuncPtrType>(&targetAddr))                          \
                return trampoline = hook::hookFunc(sdkOriginal, &HookName::detour);               \
            LogBox::Error(L"Target addr [" #targetAddr "] not found!");                           \
            return false;                                                                         \
        }                                                                                         \
                                                                                                  \
        static void unhook() {                                                                    \
            hook::unhookFunc(sdkOriginal);                                                        \
        }                                                                                         \
    };                                                                                            \
    inline RetType HookName::detour(__VA_ARGS__)

#define HOOK_TYPE_CONST(HookName, TypeName, targetAddr, RetType, ...)                             \
    class HookName : public TypeName {                                                            \
        using FuncPtrType = RetType (TypeName::*)(__VA_ARGS__) const;                             \
        static_assert(hook::is_func_ptr<FuncPtrType>, #targetAddr " should be function pointer"); \
                                                                                                  \
        inline static FuncPtrType sdkOriginal = nullptr;                                          \
        inline static FuncPtrType trampoline = nullptr;                                           \
                                                                                                  \
        template <typename... Args>                                                               \
        RetType origin(Args &&...args) const                                                      \
            requires(std::is_invocable_v<FuncPtrType, TypeName, Args...>)                         \
        {                                                                                         \
            return (this->*trampoline)(std::forward<Args>(args)...);                              \
        }                                                                                         \
        RetType detour(__VA_ARGS__);                                                              \
                                                                                                  \
    public:                                                                                       \
        static bool hook() {                                                                      \
            if (sdkOriginal = core::getOrigin<FuncPtrType>(&targetAddr))                          \
                return trampoline = hook::hookFunc(sdkOriginal, &HookName::detour);               \
            LogBox::Error(L"Target addr [" #targetAddr "] not found!");                           \
            return false;                                                                         \
        }                                                                                         \
                                                                                                  \
        static void unhook() {                                                                    \
            hook::unhookFunc(sdkOriginal);                                                        \
        }                                                                                         \
    };                                                                                            \
    inline RetType HookName::detour(__VA_ARGS__)
