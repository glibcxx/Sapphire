#include "Runtime.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hModule);
        std::thread{[]() {
            sapphire::core::Runtime::getInstance().init();
        }}.detach();
        break;
    }
    case DLL_PROCESS_DETACH:
        break;
    default:
        break;
    }
    return TRUE;
}