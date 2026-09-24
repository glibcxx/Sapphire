#pragma once

#include "common/sys/MiniWindows.h"
#include <string>

namespace sapphire::launcher {

    class DebuggerApp {
        sys::win::dword_t mGameProcessId;
        std::wstring      mLauncherAppPath;

    public:
        DebuggerApp(sys::win::dword_t pid);
        ~DebuggerApp() noexcept;

        int run();
    };

} // namespace sapphire::launcher