#include <thread>
#include <Windows.h>
#include <winuser.h>

#include "Bootloader.h"

std::unique_ptr<sapphire::bootloader::Bootloader> gBootloader;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
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