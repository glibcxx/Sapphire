#pragma once

#include <Windows.h>
#include <Psapi.h>

#include <string>
#include "ApiManager.h"
#include "PluginManager.h"

namespace moduleInfo {
    SPHR_API extern HWND     gMainWindow;
    SPHR_API extern uint64_t gStartTime;

} // namespace moduleInfo

namespace sapphire {

    class SapphireModuleInfo {
    public:
        HMODULE               sapphireModuleHandle = nullptr;
        MODULEINFO            sapphireModuleInfo;
        std::filesystem::path sapphireHome;

        SapphireModuleInfo();

        SPHR_API static SapphireModuleInfo &get();
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

        SPHR_API static Core &getInstance();

        uintptr_t getImagebase() {
            return (uintptr_t)this->mMainModuleHandle;
        }

        PluginManager &getPluginManager() {
            return this->mPluginManager;
        }

        void init();
    };

} // namespace sapphire
