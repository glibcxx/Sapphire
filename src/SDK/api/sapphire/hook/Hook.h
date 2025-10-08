#pragma once

#include <array>
#include <unordered_map>

#include "macros/Macros.h"
#include "util/Memory.hpp"
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
        SPHR_API static HookManager &getInstance();

        void teardown();

        SPHR_API bool hook(uintptr_t target, uintptr_t detour, HookPriority priority, uintptr_t &trampoline);
        SPHR_API void unhook(uintptr_t target, uintptr_t detour, HookPriority priority);

        template <typename T, typename U, typename Trampline>
        bool hook(T target, U detour, HookPriority priority, Trampline &trampoline) {
            return this->hook(
                memory::toRawFunc(target), memory::toRawFunc(detour), priority, *(uintptr_t *)(&trampoline)
            );
        }

        template <typename T, typename U>
        void unhook(T target, U detour, HookPriority priority) {
            this->unhook(std::bit_cast<uintptr_t>(target), memory::toRawFunc(detour), priority);
        }
    };

} // namespace sapphire::inline hook

#define IMPL_HOOK(Static, Const, FPtr, Call, HookName, TypeName, Priority, targetFunc, RetType, IndirectHook, ...) \
    class HookName : public TypeName {                                                                             \
        using FuncPtrType = RetType FPtr(__VA_ARGS__) Const;                                                       \
        ASSERT_VALID_OVERLOAD("Invalid overload of function: [ " #targetFunc " ], "                                \
                              "please check whether params and const qualifier of function match original!",       \
                              (FuncPtrType),                                                                       \
                              targetFunc);                                                                         \
                                                                                                                   \
        inline static uintptr_t   sdkOriginal = 0;                                                                 \
        inline static FuncPtrType trampoline = nullptr;                                                            \
                                                                                                                   \
        template <typename... Args>                                                                                \
        Static RetType origin(Args &&...args) Const {                                                              \
            return Call(std::forward<Args>(args)...);                                                              \
        }                                                                                                          \
                                                                                                                   \
        Static RetType detour(__VA_ARGS__) Const;                                                                  \
                                                                                                                   \
    public:                                                                                                        \
        static bool hook() {                                                                                       \
            auto &hookMgr = sapphire::hook::HookManager::getInstance();                                            \
            if constexpr (IndirectHook || util::type_traits::is_virtual_thunk<(FuncPtrType) & targetFunc>) {       \
                auto &apiMgr = sapphire::ApiManager::getInstance();                                                \
                if ((sdkOriginal = apiMgr.findTarget<(FuncPtrType) & targetFunc>())) {                             \
                    Logger::Debug("[Hook] Target [" #targetFunc "] found at {:#X}!", sdkOriginal);                 \
                    return hookMgr.hook(sdkOriginal, &HookName::detour, Priority, trampoline);                     \
                }                                                                                                  \
                Logger::Error("[Hook] Target [" #targetFunc "] not found! The SDK might be broken.");              \
                return false;                                                                                      \
            } else {                                                                                               \
                sdkOriginal = memory::toRawFunc((FuncPtrType) & targetFunc);                                       \
                return hookMgr.hook(sdkOriginal, &HookName::detour, Priority, trampoline);                         \
            }                                                                                                      \
        }                                                                                                          \
                                                                                                                   \
        static void unhook() {                                                                                     \
            sapphire::hook::HookManager::getInstance().unhook(                                                     \
                sdkOriginal, &HookName::detour, Priority                                                           \
            );                                                                                                     \
        }                                                                                                          \
    };                                                                                                             \
    inline RetType HookName::detour(__VA_ARGS__) Const

#define HOOK_STATIC(HookName, Priority, targetFunc, RetType, ...) \
    IMPL_HOOK(static, , (*), trampoline, HookName, sapphire::DummyClass2, Priority, targetFunc, RetType, false, __VA_ARGS__)

#define HOOK_TYPE(HookName, TypeName, Priority, targetFunc, RetType, ...) \
    IMPL_HOOK(, , (TypeName::*), (this->*trampoline), HookName, TypeName, Priority, targetFunc, RetType, false, __VA_ARGS__)

#define HOOK_TYPE_CONST(HookName, TypeName, Priority, targetFunc, RetType, ...) \
    IMPL_HOOK(, const, (TypeName::*), (this->*trampoline), HookName, TypeName, Priority, targetFunc, RetType, false, __VA_ARGS__)

// 什么叫indirect hook？就是被 hook 的函数地址不是 &funcName 表达式取得的地址，而是从 ApiManager 查表拿到的地址。
// SDK 内部必须使用 indirect hook，虚函数需要indirect hook (因为取虚函数的地址得到的是virtual thunk的地址)。
// 但是虚函数会自动使用 indirect hook，不必换为 INDIRECT_HOOK_xxx。

#define INDIRECT_HOOK_STATIC(HookName, Priority, targetFunc, RetType, ...) \
    IMPL_HOOK(static, , (*), trampoline, HookName, sapphire::DummyClass2, Priority, targetFunc, RetType, true, __VA_ARGS__)

#define INDIRECT_HOOK_TYPE(HookName, TypeName, Priority, targetFunc, RetType, ...) \
    IMPL_HOOK(, , (TypeName::*), (this->*trampoline), HookName, TypeName, Priority, targetFunc, RetType, true, __VA_ARGS__)

#define INDIRECT_HOOK_TYPE_CONST(HookName, TypeName, Priority, targetFunc, RetType, ...) \
    IMPL_HOOK(, const, (TypeName::*), (this->*trampoline), HookName, TypeName, Priority, targetFunc, RetType, true, __VA_ARGS__)