#pragma once

#include <AccCtrl.h>
#include <string>
#include "common/Expected.hpp"

namespace sapphire::launcher {

    class LauncherApp {
        std::wstring mLauncherAppPath;

    public:
        LauncherApp();

        ~LauncherApp() noexcept;

        int run();

        sapphire::Expected<DWORD, HRESULT> launchUWP(const std::wstring &appUserModelId);

        static std::wstring packageFullNameFromFamilyName(const std::wstring &familyName);
    };

} // namespace sapphire::launcher