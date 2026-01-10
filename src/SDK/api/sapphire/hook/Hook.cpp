#include "Hook.h"

#include <MinHook.h>

namespace sapphire::inline hook {

    struct FuncHookInfo {
        uintptr_t  mDetour;
        uintptr_t *mTrampoline;

        constexpr bool operator==(const FuncHookInfo &rhs) const { return this->mDetour == rhs.mDetour; }
        constexpr bool operator==(uintptr_t rhs) const { return this->mDetour == rhs; }
    };

    class HookManager::PrioritizedHookFuncList {
        static constexpr size_t PRIORITY_COUNT = static_cast<size_t>(hook::HookPriority::Count);

        std::array<std::vector<FuncHookInfo>, PRIORITY_COUNT> mCallStack;

    public:
        constexpr std::vector<FuncHookInfo> &operator[](hook::HookPriority priority) noexcept {
            return this->mCallStack[static_cast<size_t>(priority)];
        }

        constexpr FuncHookInfo &front() noexcept {
            for (auto &&it : this->mCallStack) {
                if (!it.empty())
                    return it.front();
            }
            _STL_UNREACHABLE;
        }

        constexpr FuncHookInfo &back() noexcept {
            for (auto &&it = this->mCallStack.rbegin(); it != this->mCallStack.rend(); ++it) {
                if (!it->empty())
                    return it->back();
            }
            _STL_UNREACHABLE;
        }

        constexpr bool empty() const noexcept {
            for (auto &&it : this->mCallStack) {
                if (!it.empty())
                    return false;
            }
            return true;
        }

        constexpr FuncHookInfo &insert(hook::HookPriority priority, FuncHookInfo info) {
            auto         &res = this->mCallStack[static_cast<size_t>(priority)].emplace_back(info);
            FuncHookInfo *last = nullptr;
            for (auto &&it : this->mCallStack) {
                for (auto &&info : it) {
                    if (last && *last->mTrampoline != info.mDetour) {
                        if (!*info.mTrampoline)
                            *info.mTrampoline = *last->mTrampoline;
                        *last->mTrampoline = info.mDetour;
                        break;
                    }
                    last = &info;
                }
            }
            return res;
        }

        constexpr void erase(uintptr_t target, uintptr_t detour, HookPriority priority) {
            FuncHookInfo *last = nullptr;
            for (auto &&it : this->mCallStack) {
                for (auto jt = it.begin(); jt != it.end(); ++jt) {
                    auto &&info = *jt;
                    if (info.mDetour != detour) {
                        last = &info;
                        continue;
                    }
                    if (!last) {
                        MH_RemoveHook((LPVOID)target);
                        MH_STATUS res = MH_CreateHook((LPVOID)target, (LPVOID)info.mTrampoline, (LPVOID *)&this->back().mTrampoline);
                        if (res != MH_STATUS::MH_OK) {
                            // emm...
                        }
                        res = MH_EnableHook((LPVOID)target);
                        if (res != MH_STATUS::MH_OK) {
                            // emm...
                        }
                    } else {
                        *last->mTrampoline = *info.mTrampoline;
                    }
                    it.erase(jt);
                    return;
                }
            }
        }
    };

    HookManager &HookManager::getInstance() {
        static HookManager h;
        return h;
    }

    void HookManager::teardown() {
        if (!this->mInitialized) return;
        MH_Uninitialize();
        this->mInitialized = false;
    }

    HookManager::HookManager() {
        MH_Initialize();
        this->mInitialized = true;
    }

    HookManager::~HookManager() {
        this->teardown();
    }

    bool HookManager::hook(
        uintptr_t target, uintptr_t detour, HookPriority priority, uintptr_t &trampoline
    ) {
        if (!this->mInitialized || !target) return false;
        auto found = this->mHookedFunctions.find(target);
        trampoline = 0;
        if (found == this->mHookedFunctions.end()) {
            MH_STATUS res = MH_CreateHook((LPVOID)target, (LPVOID)detour, (LPVOID *)&trampoline);
            if (res != MH_STATUS::MH_OK) return false;
            found = this->mHookedFunctions.emplace(target, PrioritizedHookFuncList{}).first;
        } else if (priority == HookPriority{0} && found->second[HookPriority{0}].empty()) {
            MH_RemoveHook((LPVOID)target);
            MH_STATUS res = MH_CreateHook((LPVOID)target, (LPVOID)detour, (LPVOID *)&trampoline);
            if (res != MH_STATUS::MH_OK) return false;
            FuncHookInfo &last = found->second.back();
            *last.mTrampoline = trampoline;
            trampoline = 0;
        }
        found->second.insert(priority, {detour, &trampoline});
        MH_STATUS res = MH_EnableHook((LPVOID)target);
        if (res != MH_OK && res != MH_ERROR_ENABLED) {
            unhook(target, detour, priority);
            return false;
        }

        return true;
    }

    void HookManager::unhook(uintptr_t target, uintptr_t detour, HookPriority priority) {
        if (!this->mInitialized || !target) return;
        auto found = this->mHookedFunctions.find(target);
        if (found == this->mHookedFunctions.end()) return;
        PrioritizedHookFuncList &list = found->second;
        list.erase(target, detour, priority);
        if (list.empty()) this->mHookedFunctions.erase(found);
    }

} // namespace sapphire::inline hook