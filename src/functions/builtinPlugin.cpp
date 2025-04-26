#include <thread>
#include <Windows.h>

#include "SDK/core/Core.h"

#include "guioverlay/DX12Hook.h"
#include "tickrate/TickRateTest.h"
#include "smoothpiston/SmoothPiston.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        hook::init();
        std::thread{[]() { InstallDX12Hook(); }}.detach();
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