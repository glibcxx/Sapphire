#include "builtinPlugin.h"

#include "SDK/api/sapphire/hook/Hook.h"

#include "tickrate/TickRateTest.h"
#include "smoothpiston/SmoothPiston.h"
#include "chunkborder/ChunkBorderRenderer.h"

#include "SDK/api/src-client/common/client/renderer/rendergraph/PlayerRenderView.h"

namespace builtinPluginInfo {

    HMODULE hModule = nullptr;

} // namespace builtinPluginInfo

HOOK_TYPE_CONST(
    TestRender,
    PlayerRenderView::lambda_75d793f24d52434fc09af39da3e425e3,
    hook::HookPriority::Normal,
    PlayerRenderView::lambda_75d793f24d52434fc09af39da3e425e3::operator(),
    void,
    const PlayerRenderView::LegacyPlayerRenderPass &data,
    rendergraph::RenderContext                     &renderContext
) {
    return this->origin(data, renderContext);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        builtinPluginInfo::hModule = hModule;
        TickRatePlugin::getInstance();
        SmoothPistonPlugin::getInstance();
        installChunkBorderRender();
        TestRender::hook();
        break;
    case DLL_PROCESS_DETACH:
        TestRender::unhook();
        FreeLibraryAndExitThread(hModule, TRUE);
        break;
    default:
        break;
    }
    return TRUE;
}