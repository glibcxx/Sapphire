#pragma once

#include <array>
#include <unordered_map>

#include "macros/Macros.h"
#include "common/Memory.hpp"
#include "common/TypeTraits.hpp"
#include "SDK/core/SymbolResolver.h"
#include "SDK/api/sapphire/hook/SafeHook.h"
#include "SDK/api/sapphire/logger/Logger.h"

namespace sapphire::inline hook {

    enum class HookPriority : uint8_t {
        High = 0,
        Normal,
        Low,
        Count
    };

    class DummyClass {};

    class HookManager {
        class PrioritizedHookFuncList;

        std::unordered_map<uintptr_t, PrioritizedHookFuncList> mHookedFunctions;
        bool                                                   mInitialized = false;

        HookManager();
        ~HookManager();

    public:
        SPHR_API static HookManager &getInstance();

        void teardown();

        SPHR_API bool hook(uintptr_t target, uintptr_t detour, HookPriority priority, uintptr_t &trampoline);
        SPHR_API void unhook(uintptr_t target, uintptr_t detour, HookPriority priority);

        template <typename T, typename U, typename Trampline>
        bool hook(T target, U detour, HookPriority priority, Trampline &trampoline) {
            return this->hook(
                sapphire::toFuncAddr(target), sapphire::toFuncAddr(detour), priority, *(uintptr_t *)(&trampoline)
            );
        }

        template <typename T, typename U>
        void unhook(T target, U detour, HookPriority priority) {
            this->unhook(std::bit_cast<uintptr_t>(target), sapphire::toFuncAddr(detour), priority);
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
                                                                                                             \
        inline static uintptr_t   sdkOriginal = 0;                                                           \
        inline static FuncPtrType trampoline = nullptr;                                                      \
                                                                                                             \
        Static RetType detour(__VA_ARGS__) Const;                                                            \
                                                                                                             \
    public:                                                                                                  \
        template <typename... Args>                                                                          \
        Static RetType origin(Args &&...args) Const {                                                        \
            return Call(std::forward<Args>(args)...);                                                        \
        }                                                                                                    \
                                                                                                             \
        static bool hook() {                                                                                 \
            auto &hookMgr = sapphire::hook::HookManager::getInstance();                                      \
            if constexpr (sapphire::isVirtualThunk<(FuncPtrType) & targetFunc>) {                            \
                auto &apiMgr = sapphire::SymbolResolver::getInstance();                                      \
                sdkOriginal = apiMgr.findTarget<(FuncPtrType) & targetFunc>();                               \
            } else {                                                                                         \
                sdkOriginal = sapphire::toFuncAddr((FuncPtrType) & targetFunc);                              \
            }                                                                                                \
            if (!sdkOriginal)                                                                                \
                sapphire::error("[Hook] Target [" #targetFunc "] not found! The SDK might be broken.");      \
            return hookMgr.hook(sdkOriginal, &HookName::detour, Priority, trampoline);                       \
        }                                                                                                    \
                                                                                                             \
        static void unhook() {                                                                               \
            sapphire::hook::HookManager::getInstance().unhook(                                               \
                sdkOriginal, &HookName::detour, Priority                                                     \
            );                                                                                               \
        }                                                                                                    \
    };                                                                                                       \
    inline RetType HookName::detour(__VA_ARGS__) Const

#define HOOK_STATIC(HookName, Priority, targetFunc, RetType, ...) \
    IMPL_HOOK(static, , (*), trampoline, HookName, sapphire::DummyClass, Priority, targetFunc, RetType, __VA_ARGS__)

#define HOOK_TYPE(HookName, TypeName, Priority, targetFunc, RetType, ...) \
    IMPL_HOOK(, , (TypeName::*), (this->*trampoline), HookName, TypeName, Priority, targetFunc, RetType, __VA_ARGS__)

#define HOOK_TYPE_CONST(HookName, TypeName, Priority, targetFunc, RetType, ...) \
    IMPL_HOOK(, const, (TypeName::*), (this->*trampoline), HookName, TypeName, Priority, targetFunc, RetType, __VA_ARGS__)
