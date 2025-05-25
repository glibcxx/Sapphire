#include "builtinPlugin.h"

#include <thread>
#include <map>

#include "SDK/api/sapphire/hook/Hook.h"

#include "tickrate/TickRateTest.h"
#include "smoothpiston/SmoothPiston.h"
#include "chunkborder/ChunkBorderRenderer.h"

namespace builtinPluginInfo {

    HMODULE hModule = nullptr;

} // namespace builtinPluginInfo

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        builtinPluginInfo::hModule = hModule;
        TickRatePlugin::getInstance();
        SmoothPistonPlugin::getInstance();
        installChunkBorderRender();
        break;
    case DLL_PROCESS_DETACH:
        FreeLibraryAndExitThread(hModule, TRUE);
        break;
    default:
        break;
    }
    return TRUE;
}