#pragma once

#include "common/IPC/Pipe.h"
#include "common/coroutine/IoContext.hpp"
#include "common/coroutine/Task.hpp"
#include "common/sys/MiniWindows.h"
#include <filesystem>

namespace sapphire::launcher {

    class DebuggerApp {
        sys::win::dword_t     mGameProcessId;
        std::wstring          mLauncherAppPath;
        std::filesystem::path mLauncherAppDir;
        sys::win::handle_t    mGameProcessHandle;

    public:
        bool mReconnectNeeded = false;
        bool mInjectSuccess = false;

        DebuggerApp(sys::win::dword_t pid);
        ~DebuggerApp() noexcept;

        int run();

        void ensureDataDirExist();

        coro::Task<int> pipeServerMain(coro::IoContext &ctx);
        coro::Task<int> handlePipeConnection(coro::IoContext &ctx, ipc::backend::Pipe pipe);

        static sys::win::dword_t setPermissions(const std::wstring &wstrFilePath, sys::win::dword_t permission);

        static bool injectDll(sys::win::handle_t hProcess, const std::filesystem::path &dllPath);

        static void disableDebugging(const std::wstring &familyName);
    };

} // namespace sapphire::launcher