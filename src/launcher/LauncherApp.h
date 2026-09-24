#pragma once

#include <AccCtrl.h>
#include <atlcomcli.h>
#include <filesystem>
#include <shobjidl_core.h>
#include <string>
#include "common/Expected.hpp"
#include "common/coroutine/IoContext.hpp"
#include "common/coroutine/Task.hpp"
#include "common/IPC/Pipe.h"
#include "common/sys/MiniWindows.h"

namespace sapphire::launcher {

    class LauncherApp {
        sys::win::dword_t                     mGameProcessId;
        std::wstring                          mLauncherAppPath;
        std::filesystem::path                 mLauncherAppDir;
        sys::win::handle_t                    mGameProcessHandle;
        ATL::CComQIPtr<IPackageDebugSettings> mDebugSettings;
        std::wstring                          mAppFullName;
        std::jthread                          mLaunchThread;

    public:
        bool mReconnectNeeded = false;
        bool mInjectSuccess = false;

        LauncherApp();

        ~LauncherApp() noexcept;

        int run();

        coro::Task<bool> launchGame(coro::IoContext &ctx, const std::wstring &familyName, ipc::backend::Pipe pipe);

        coro::Task<int> pipeServerMain(coro::IoContext &ctx);
        coro::Task<int> handlePipeConnection(coro::IoContext &ctx, ipc::backend::Pipe pipe);

        void ensureDataDirExist();

        static sys::win::dword_t setPermissions(const std::wstring &wstrFilePath, sys::win::dword_t permission);

        Expected<DWORD, HRESULT> launchUWP(const std::wstring &appUserModelId);

        static std::wstring packageFullNameFromFamilyName(const std::wstring &familyName);

        static bool injectDll(sys::win::handle_t hProcess, const std::filesystem::path &dllPath);

        static void disableDebugging(const std::wstring &familyName);
    };

} // namespace sapphire::launcher