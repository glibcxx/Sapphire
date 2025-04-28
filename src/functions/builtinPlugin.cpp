#include <thread>
#include <Windows.h>

#include "SDK/core/Core.h"

#include "guioverlay/DX12Hook.h"
#include "tickrate/TickRateTest.h"
#include "smoothpiston/SmoothPiston.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        hook::init();
        std::thread{[]() {
            if (!InstallDX12Hook())
                LogBox::Error(L"DX12 Hook 安装失败！");
        }}.detach();
        installTickRate();
        installSmoothPiston();
        break;
    case DLL_PROCESS_DETACH:
        uninstallSmoothPiston();
        uninstallTickRate();
        UninstallDX12Hook();
        hook::uninit();
        FreeLibraryAndExitThread(hModule, TRUE);
        break;
    default:
        break;
    }
    return TRUE;
}