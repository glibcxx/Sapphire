#include "builtinPlugin.h"

#include <thread>

#include "SDK/core/Core.h"
#include "SDK/api/src-client/common/client/game/ClientInstance.h"

#include "guioverlay/DX12Hook.h"
#include "tickrate/TickRateTest.h"
#include "smoothpiston/SmoothPiston.h"

namespace builtinPluginInfo {

    HMODULE hModule = nullptr;

} // namespace builtinPluginInfo

static void makeClientInstance(
    ClientInstance *ret, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t a8, uint64_t a9, uint64_t a10
) {
    core::ApiLoader<
#if MC_VERSION == v1_21_2
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x49\x8B\xF9\x49\x8B\xF0\x48\x8B\xEA"_sig,
#elif MC_VERSION == v1_21_50
        "\x48\x89\x5C\x24\x00\x57\x48\x81\xEC\x00\x00\x00\x00\x4C\x8B\xDA\x48\x8B\xD9\x4C\x8B\x94\x24"_sig,
#elif MC_VERSION == v1_21_60
        "\x48\x8B\xC4\x48\x89\x58\x00\x48\x89\x68\x00\x48\x89\x70\x00\x57\x48\x81\xEC\x00\x00\x00\x00\x49\x8B\xD9"_sig,
#endif
        &makeClientInstance>::origin(ret, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

HOOK_STATIC(
    GetClientInstance,
    makeClientInstance,
    void,
    ClientInstance *ret,
    uint64_t        a2,
    uint64_t        a3,
    uint64_t        a4,
    uint64_t        a5,
    uint64_t        a6,
    uint64_t        a7,
    uint64_t        a8,
    uint64_t        a9,
    uint64_t        a10
) {
    origin(ret, a2, a3, a4, a5, a6, a7, a8, a9, a10);
    ClientInstance::primaryClientInstance = ret;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        builtinPluginInfo::hModule = hModule;
        hook::init();
        GetClientInstance::hook();
        std::thread{[]() {
            if (!InstallDX12Hook())
                Logger::ErrorBox(L"DX12 Hook 安装失败！");
        }}.detach();
        installTickRate();
        installSmoothPiston();
        break;
    case DLL_PROCESS_DETACH:
        uninstallSmoothPiston();
        uninstallTickRate();
        UninstallDX12Hook();
        GetClientInstance::unhook();
        hook::uninit();
        FreeLibraryAndExitThread(hModule, TRUE);
        break;
    default:
        break;
    }
    return TRUE;
}