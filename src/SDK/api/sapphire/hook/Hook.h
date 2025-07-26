#pragma once

#include <array>
#include <unordered_map>

#include "macros/Macros.h"
#include "util/TypeTraits.hpp"
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
        bool                                                   mInitialized = false;

        HookManager();
        ~HookManager();

    public:
        SDK_API static HookManager &getInstance();

        void teardown();

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

#define IMPL_HOOK(Static, Const, FPtr, Call, HookName, TypeName, Priority, targetFunc, RetType, ...)         \
    class HookName : public TypeName {                                                                       \
        using FuncPtrType = RetType FPtr(__VA_ARGS__) Const;                                                 \
        ASSERT_VALID_OVERLOAD("Invalid overload of function: [ " #targetFunc " ], "                          \
                              "please check whether params and const qualifier of function match original!", \
                              (FuncPtrType),                                                                 \
                              targetFunc);                                                                   \
        ASSERT_HOOKABLE("Function [ " #targetFunc " ] is not hookable!", (FuncPtrType) & targetFunc)         \
                                                                                                             \
        inline static uintptr_t   sdkOriginal = 0;                                                           \
        inline static FuncPtrType trampoline = nullptr;                                                      \
                                                                                                             \
        template <typename... Args>                                                                          \
        Static RetType origin(Args &&...args) Const {                                                        \
            return Call(std::forward<Args>(args)...);                                                        \
        }                                                                                                    \
                                                                                                             \
        Static RetType detour(__VA_ARGS__) Const;                                                            \
                                                                                                             \
    public:                                                                                                  \
        static bool hook() {                                                                                 \
            auto &hookMgr = sapphire::hook::HookManager::getInstance();                                      \
            auto &apiMgr = sapphire::ApiManager::getInstance();                                              \
            if (sdkOriginal = apiMgr.findTarget(                                                             \
                    util::ApiUniqueId::make<(FuncPtrType) & targetFunc>()                                    \
                )) {                                                                                         \
                Logger::Debug("[Hook] Target [" #targetFunc "] found at {:#X}!", sdkOriginal);               \
                return hookMgr.hook(sdkOriginal, &HookName::detour, Priority, trampoline);                   \
            }                                                                                                \
            Logger::Error("[Hook] Target [" #targetFunc "] not found! The SDK might be broken.");            \
            return false;                                                                                    \
        }                                                                                                    \
                                                                                                             \
        static void unhook() {                                                                               \
            sapphire::hook::HookManager::getInstance().unhook(                                               \
                sdkOriginal, &HookName::detour, Priority                                                     \
            );                                                                                               \
        }                                                                                                    \
    };                                                                                                       \
    inline RetType HookName::detour(__VA_ARGS__) Const

// Hook SDK内的函数，用下面3个

#define HOOK_STATIC(HookName, Priority, targetFunc, RetType, ...) \
    IMPL_HOOK(static, , (*), trampoline, HookName, sapphire::DummyClass2, Priority, targetFunc, RetType, __VA_ARGS__)

#define HOOK_TYPE(HookName, TypeName, Priority, targetFunc, RetType, ...) \
    IMPL_HOOK(, , (TypeName::*), (this->*trampoline), HookName, TypeName, Priority, targetFunc, RetType, __VA_ARGS__)

#define HOOK_TYPE_CONST(HookName, TypeName, Priority, targetFunc, RetType, ...) \
    IMPL_HOOK(, const, (TypeName::*), (this->*trampoline), HookName, TypeName, Priority, targetFunc, RetType, __VA_ARGS__)

#define IMPL_HOOK_RAW(Static, Const, FPtr, Call, HookName, TypeName, Priority, targetFunc, RetType, ...)     \
    class HookName : public TypeName {                                                                       \
        using FuncPtrType = RetType FPtr(__VA_ARGS__) Const;                                                 \
        ASSERT_VALID_OVERLOAD("Invalid overload of function: [ " #targetFunc " ], "                          \
                              "please check whether params and const qualifier of function match original!", \
                              (FuncPtrType),                                                                 \
                              targetFunc);                                                                   \
        inline static FuncPtrType trampoline = nullptr;                                                      \
                                                                                                             \
        template <typename... Args>                                                                          \
        Static RetType origin(Args &&...args) Const {                                                        \
            return Call(std::forward<Args>(args)...);                                                        \
        }                                                                                                    \
                                                                                                             \
        Static RetType detour(__VA_ARGS__) Const;                                                            \
                                                                                                             \
    public:                                                                                                  \
        static bool hook() {                                                                                 \
            auto &hookMgr = sapphire::hook::HookManager::getInstance();                                      \
            return hookMgr.hook((FuncPtrType) & targetFunc, &HookName::detour, Priority, trampoline);        \
        }                                                                                                    \
                                                                                                             \
        static void unhook() {                                                                               \
            sapphire::hook::HookManager::getInstance().unhook(                                               \
                (FuncPtrType) & targetFunc, &HookName::detour, Priority                                      \
            );                                                                                               \
        }                                                                                                    \
    };                                                                                                       \
    inline RetType HookName::detour(__VA_ARGS__) Const

// Hook 非 SDK 内的函数，用下面3个

#define HOOK_RAW_STATIC(HookName, Priority, targetFunc, RetType, ...) \
    IMPL_HOOK_RAW(static, , (*), trampoline, HookName, sapphire::DummyClass2, Priority, targetFunc, RetType, __VA_ARGS__)

#define HOOK_RAW_TYPE(HookName, TypeName, Priority, targetFunc, RetType, ...) \
    IMPL_HOOK_RAW(, , (TypeName::*), (this->*trampoline), HookName, TypeName, Priority, targetFunc, RetType, __VA_ARGS__)

#define HOOK_RAW_TYPE_CONST(HookName, TypeName, Priority, targetFunc, RetType, ...) \
    IMPL_HOOK_RAW(, const, (TypeName::*), (this->*trampoline), HookName, TypeName, Priority, targetFunc, RetType, __VA_ARGS__)