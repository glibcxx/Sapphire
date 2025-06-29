#include "builtinPlugin.h"

#include "SDK/api/sapphire/hook/Hook.h"

#include "tickrate/TickRateTest.h"
#include "smoothpiston/SmoothPiston.h"
#include "chunkborder/ChunkBorderRenderer.h"
#include "freecamera/FreeCamera.h"

#include "SDK/api/src-client/common/client/renderer/rendergraph/PlayerRenderView.h"

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
        FreeCameraPlugin::getInstance();
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