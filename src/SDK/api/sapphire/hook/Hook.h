#pragma once

#include <bit>
#include <array>
#include <vector>
#include <unordered_map>
#include <functional>

#include "macros/Macros.h"
#include "util/ApiUniqueId.hpp"
#include "util/TypeTraits.hpp"
#include "util/Memory.hpp"
#include "../logger/Logger.h"
#include "SDK/core/ApiManager.h"

namespace sapphire::inline hook {

    enum class HookPriority : uint8_t {
        High = 0,
        Normal,
        Low,
        Count
    };

    class DummyClass2 {};

    class HookManager {
        class PrioritizedHookFuncList;

        std::unordered_map<uintptr_t, PrioritizedHookFuncList> mHookedFunctions;

        HookManager();
        ~HookManager();

    public:
        SDK_API static HookManager &getInstance();

        SDK_API bool hook(uintptr_t target, uintptr_t detour, HookPriority priority, uintptr_t &trampoline);
        SDK_API void unhook(uintptr_t target, uintptr_t detour, HookPriority priority);

        template <typename T, typename U, typename Trampline>
        bool hook(T target, U detour, HookPriority priority, Trampline &trampoline) {
            return this->hook(std::bit_cast<uintptr_t>(target), memory::toRawFunc(detour), priority, *(uintptr_t *)(&trampoline));
        }

        template <typename T, typename U>
        void unhook(T target, U detour, HookPriority priority) {
            this->unhook(std::bit_cast<uintptr_t>(target), memory::toRawFunc(detour), priority);
        }
    };

} // namespace sapphire::inline hook

#define IMPL_HOOK(Static, Const, FPtr, Call, HookName, TypeName, Priority, targetAddr, RetType, ...) \
    class HookName : public TypeName {                                                               \
        using FuncPtrType = RetType FPtr(__VA_ARGS__) Const;                                         \
        inline static uintptr_t     sdkOriginal = 0;                                                 \
        inline static FuncPtrType   trampoline = nullptr;                                            \
                                                                                                     \
        template <typename... Args>                                                                  \
        Static RetType origin(Args &&...args) Const {                                                \
            return Call(std::forward<Args>(args)...);                                                \
        }                                                                                            \
                                                                                                     \
        Static RetType detour(__VA_ARGS__) Const;                                                    \
                                                                                                     \
    public:                                                                                          \
        static bool hook() {                                                                         \
            auto &hookMgr = sapphire::hook::HookManager::getInstance();                              \
            auto &apiMgr = sapphire::ApiManager::getInstance();                                      \
            if (sdkOriginal = apiMgr.findTarget(                                                     \
                    util::ApiUniqueId::make<(FuncPtrType) & targetAddr>()                            \
                )) {                                                                                 \
                Logger::Debug("[Hook] Target [" #targetAddr "] found at {:#X}!", sdkOriginal);       \
                return hookMgr.hook(sdkOriginal, &HookName::detour, Priority, trampoline);           \
            }                                                                                        \
            Logger::Error("[Hook] Target [" #targetAddr "] not found! The SDK might be broken.");    \
            return false;                                                                            \
        }                                                                                            \
                                                                                                     \
        static void unhook() {                                                                       \
            sapphire::hook::HookManager::getInstance().unhook(                                       \
                sdkOriginal, &HookName::detour, Priority                                             \
            );                                                                                       \
        }                                                                                            \
    };                                                                                               \
    inline RetType HookName::detour(__VA_ARGS__) Const

// Hook SDK内的函数，用下面3个

#define HOOK_STATIC(HookName, Priority, targetAddr, RetType, ...) \
    IMPL_HOOK(static, , (*), trampoline, HookName, sapphire::DummyClass2, Priority, targetAddr, RetType, __VA_ARGS__)

#define HOOK_TYPE(HookName, TypeName, Priority, targetAddr, RetType, ...) \
    IMPL_HOOK(, , (TypeName::*), (this->*trampoline), HookName, TypeName, Priority, targetAddr, RetType, __VA_ARGS__)

#define HOOK_TYPE_CONST(HookName, TypeName, Priority, targetAddr, RetType, ...) \
    IMPL_HOOK(, const, (TypeName::*), (this->*trampoline), HookName, TypeName, Priority, targetAddr, RetType, __VA_ARGS__)

#define IMPL_HOOK_RAW(Static, Const, FPtr, Call, HookName, TypeName, Priority, targetAddr, RetType, ...) \
    class HookName : public TypeName {                                                                   \
        using FuncPtrType = RetType FPtr(__VA_ARGS__) Const;                                             \
        inline static FuncPtrType   trampoline = nullptr;                                                \
                                                                                                         \
        template <typename... Args>                                                                      \
        Static RetType origin(Args &&...args) Const {                                                    \
            return Call(std::forward<Args>(args)...);                                                    \
        }                                                                                                \
                                                                                                         \
        Static RetType detour(__VA_ARGS__) Const;                                                        \
                                                                                                         \
    public:                                                                                              \
        static bool hook() {                                                                             \
            auto &hookMgr = sapphire::hook::HookManager::getInstance();                                  \
            return hookMgr.hook((FuncPtrType) & targetAddr, &HookName::detour, Priority, trampoline);    \
        }                                                                                                \
                                                                                                         \
        static void unhook() {                                                                           \
            sapphire::hook::HookManager::getInstance().unhook(                                           \
                (FuncPtrType) & targetAddr, &HookName::detour, Priority                                  \
            );                                                                                           \
        }                                                                                                \
    };                                                                                                   \
    inline RetType HookName::detour(__VA_ARGS__) Const

// Hook 非 SDK 内的函数，用下面3个

#define HOOK_RAW_STATIC(HookName, Priority, targetAddr, RetType, ...) \
    IMPL_HOOK_RAW(static, , (*), trampoline, HookName, sapphire::DummyClass2, Priority, targetAddr, RetType, __VA_ARGS__)

#define HOOK_RAW_TYPE(HookName, TypeName, Priority, targetAddr, RetType, ...) \
    IMPL_HOOK_RAW(, , (TypeName::*), (this->*trampoline), HookName, TypeName, Priority, targetAddr, RetType, __VA_ARGS__)

#define HOOK_RAW_TYPE_CONST(HookName, TypeName, Priority, targetAddr, RetType, ...) \
    IMPL_HOOK_RAW(, const, (TypeName::*), (this->*trampoline), HookName, TypeName, Priority, targetAddr, RetType, __VA_ARGS__)