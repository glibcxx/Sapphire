#include "LauncherApp.h"
#include "Logger.h"

#include <atlcomcli.h>
#include <aclapi.h>
#include <iostream>
#include <sddl.h>
#include <appmodel.h>
#include <shobjidl.h>
#include <processthreadsapi.h>
#include <stdexcept>

namespace sapphire::launcher {

    LauncherApp::LauncherApp() : mLauncherAppPath(MAX_PATH, {}) {
        HRESULT hResult = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        if (FAILED(hResult)) {
            ErrorBox(L"[launcher] CoInitializeEx 失败 (code: {})", hResult);
            throw std::runtime_error{"[launcher] CoInitializeEx 失败"};
        }

        GetModuleFileNameW(nullptr, mLauncherAppPath.data(), mLauncherAppPath.size());
    }

    LauncherApp::~LauncherApp() noexcept { CoUninitialize(); }

    int LauncherApp::run() {
        std::wstring familyName = L"Microsoft.MinecraftUWP_8wekyb3d8bbwe";
        std::wstring appFullName = packageFullNameFromFamilyName(familyName);
        if (appFullName.empty()) {
            std::cerr << "[launcher] 未找到游戏，你可能未安装游戏\n";
            return -1;
        }

        ATL::CComQIPtr<IPackageDebugSettings> debugSettings;

        HRESULT hResult = debugSettings.CoCreateInstance(CLSID_PackageDebugSettings, NULL, CLSCTX_ALL);
        if (hResult != S_OK) {
            std::cerr << std::format("[launcher] 创建 PackageDebugSettings 失败 (code: {})", hResult);
            return -1;
        }

        hResult = debugSettings->EnableDebugging(appFullName.data(), mLauncherAppPath.data(), NULL);
        if (hResult != S_OK) {
            std::cerr << std::format("[launcher] 无法配置调试器 (code: {})", hResult);
            return 0;
        }

        familyName.append(L"!App");
        Expected<DWORD, HRESULT> gamePid = launchUWP(familyName);

        if (gamePid.hasError()) {
            std::cerr << std::format("[launcher] 无法启动游戏 (code: {})", gamePid.error());
        }

        debugSettings->DisableDebugging(appFullName.data());

        return 0;
    }

    Expected<DWORD, HRESULT> LauncherApp::launchUWP(const std::wstring &appUserModelId) {
        CComPtr<IApplicationActivationManager> manager;

        HRESULT result = CoCreateInstance(
            CLSID_ApplicationActivationManager,
            NULL,
            CLSCTX_LOCAL_SERVER,
            IID_IApplicationActivationManager,
            (LPVOID *)&manager
        );
        if (!SUCCEEDED(result)) return {unexpected, result};

        result = CoAllowSetForegroundWindow(manager, NULL);
        if (!SUCCEEDED(result)) return {unexpected, result};

        DWORD dwProcessId = 0;
        result = manager->ActivateApplication(appUserModelId.data(), NULL, AO_NONE, &dwProcessId);
        if (!SUCCEEDED(result)) return {unexpected, result};

        return dwProcessId;
    }

    std::wstring LauncherApp::packageFullNameFromFamilyName(
        const std::wstring &familyName
    ) {
        UINT32 count = 0, length = 0;
        LONG   status = FindPackagesByPackageFamily(familyName.c_str(), PACKAGE_FILTER_HEAD, &count, nullptr, &length, nullptr, nullptr);
        if (status == ERROR_SUCCESS || status != ERROR_INSUFFICIENT_BUFFER)
            return {};

        std::wstring buffer(length * count, L'\0');
        wchar_t     *fullname = nullptr;
        status = FindPackagesByPackageFamily(familyName.c_str(), PACKAGE_FILTER_HEAD, &count, &fullname, &length, buffer.data(), nullptr);
        if (status == ERROR_SUCCESS)
            return fullname;

        return {};
    }

} // namespace sapphire::launcher