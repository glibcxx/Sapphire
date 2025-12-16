#include <thread>
#include <Windows.h>
#include <winuser.h>

#include "macros/Macros.h"

#include "Bootloader.h"

std::unique_ptr<sapphire::bootloader::Bootloader> gBootloader;

void waitForDebuggerIfDebugMode() {
#ifdef SPHR_DEBUG
    if (IsDebuggerPresent()) return;

    MessageBoxW(nullptr, L"Waiting for debugger...\nClick 'OK' to continue.", L"Debug", MB_ICONINFORMATION);
    if (IsDebuggerPresent())
        __debugbreak();
#endif
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        waitForDebuggerIfDebugMode();
        std::thread([hModule]() {
            gBootloader = std::make_unique<sapphire::bootloader::Bootloader>(hModule);
            if (!gBootloader->preBoot())
                return;
            gBootloader->bootSapphire();
            gBootloader->postBoot();
        }).detach();
    }
    return TRUE;
}