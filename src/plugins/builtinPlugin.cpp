#include "builtinPlugin.h"

#include <thread>

#include "SDK/core/Core.h"

#include "tickrate/TickRateTest.h"
#include "smoothpiston/SmoothPiston.h"

namespace builtinPluginInfo {

    HMODULE hModule = nullptr;

} // namespace builtinPluginInfo

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        builtinPluginInfo::hModule = hModule;
        hook::init();
        installTickRate();
        installSmoothPiston();
        break;
    case DLL_PROCESS_DETACH:
        uninstallSmoothPiston();
        uninstallTickRate();
        hook::uninit();
        FreeLibraryAndExitThread(hModule, TRUE);
        break;
    default:
        break;
    }
    return TRUE;
}