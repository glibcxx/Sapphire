#pragma once

#include <Windows.h>
#include <Psapi.h>

#include <string>
#include "util/StringLiteral.hpp"
#include "util/Memory.hpp"
#include "ApiManager.h"
#include "PluginManager.h"

namespace moduleInfo {
    SDK_API extern HWND     gMainWindow;
    SDK_API extern uint64_t gStartTime;

} // namespace moduleInfo

namespace sapphire {

    class SapphireModuleInfo {
    public:
        HMODULE               sapphireModuleHandle = nullptr;
        MODULEINFO            sapphireModuleInfo;
        std::filesystem::path sapphireHome;

        SapphireModuleInfo();

        SDK_API static SapphireModuleInfo &get();
    };

    class Core {
        PluginManager mPluginManager;

        HMODULE mMainModuleHandle = GetModuleHandleW(nullptr);
        bool    mInitialized = false;

        bool loadAllPlugins();

        bool _init();

        void _uninit() noexcept;

    public:
        Core() = default;
        ~Core() noexcept;
        Core(const Core &) = delete;
        Core &operator=(const Core &) = delete;

        SDK_API static Core &getInstance();

        uintptr_t getImagebase() {
            return (uintptr_t)this->mMainModuleHandle;
        }

        PluginManager &getPluginManager() {
            return this->mPluginManager;
        }

        void init();
    };

} // namespace sapphire
