#include "RuntimeLinker.h"
#include "MinHook.h"
#include <Windows.h>
#include <winnt.h>
#include <winternl.h>

#define LDR_DLL_NOTIFICATION_REASON_LOADED 1
#define LDR_DLL_NOTIFICATION_REASON_UNLOADED 2

typedef struct _LDR_DLL_LOADED_NOTIFICATION_DATA {
    ULONG           Flags;       // Reserved.
    PUNICODE_STRING FullDllName; // The full path name of the DLL module.
    PUNICODE_STRING BaseDllName; // The base file name of the DLL module.
    PVOID           DllBase;     // A pointer to the base address for the DLL in memory.
    ULONG           SizeOfImage; // The size of the DLL image, in bytes.
} LDR_DLL_LOADED_NOTIFICATION_DATA, *PLDR_DLL_LOADED_NOTIFICATION_DATA;

typedef struct _LDR_DLL_UNLOADED_NOTIFICATION_DATA {
    ULONG           Flags;       // Reserved.
    PUNICODE_STRING FullDllName; // The full path name of the DLL module.
    PUNICODE_STRING BaseDllName; // The base file name of the DLL module.
    PVOID           DllBase;     // A pointer to the base address for the DLL in memory.
    ULONG           SizeOfImage; // The size of the DLL image, in bytes.
} LDR_DLL_UNLOADED_NOTIFICATION_DATA, *PLDR_DLL_UNLOADED_NOTIFICATION_DATA;

typedef union _LDR_DLL_NOTIFICATION_DATA {
    LDR_DLL_LOADED_NOTIFICATION_DATA   Loaded;
    LDR_DLL_UNLOADED_NOTIFICATION_DATA Unloaded;
} LDR_DLL_NOTIFICATION_DATA, *PLDR_DLL_NOTIFICATION_DATA;

typedef const PLDR_DLL_NOTIFICATION_DATA PCLDR_DLL_NOTIFICATION_DATA;

typedef VOID(CALLBACK *PLDR_DLL_NOTIFICATION_FUNCTION)(
    ULONG                      NotificationReason,
    PLDR_DLL_NOTIFICATION_DATA NotificationData,
    PVOID                      Context
);

typedef struct _LDR_DLL_NOTIFICATION_ENTRY {
    LIST_ENTRY                     List;
    PLDR_DLL_NOTIFICATION_FUNCTION Callback;
    PVOID                          Context;
} LDR_DLL_NOTIFICATION_ENTRY, *PLDR_DLL_NOTIFICATION_ENTRY;

typedef NTSTATUS(NTAPI *PLDR_REGISTER_DLL_NOTIFICATION)(
    ULONG                          Flags,
    PLDR_DLL_NOTIFICATION_FUNCTION NotificationFunction,
    PVOID                          Context,
    PVOID                         *Cookie
);

BOOL WINAPI FakeDllMain(HMODULE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    return FALSE;
}

void sapphire::bootloader::RuntimeLinker::forceDllMainToFail(RuntimeLinker *self, HMODULE hDll) {
    PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)hDll;
    PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)(hDll + pDos->e_lfanew);

    if (pNt->OptionalHeader.AddressOfEntryPoint == 0) return;
    LPVOID pEntryPoint = (LPVOID)(hDll + pNt->OptionalHeader.AddressOfEntryPoint);

    if (MH_CreateHook(pEntryPoint, (LPVOID)&FakeDllMain, NULL) != MH_OK) {
        self->mIPCClient.send(ipc::status::Error, "Failed to create EntryPoint hook.");
        return;
    }
    if (MH_EnableHook(pEntryPoint) != MH_OK) {
        self->mIPCClient.send(ipc::status::Error, "Failed to enable EntryPoint hook.");
        return;
    }
}

VOID CALLBACK LdrDllNotification(
    ULONG                       NotificationReason,
    PCLDR_DLL_NOTIFICATION_DATA NotificationData,
    PVOID                       Context
) {
    if (NotificationReason == LDR_DLL_NOTIFICATION_REASON_LOADED) {
        auto runtimeLinker = static_cast<sapphire::bootloader::RuntimeLinker *>(Context);
        if (!runtimeLinker->getIatPatcher().patchModule(
                (HMODULE)NotificationData->Loaded.DllBase, runtimeLinker->getSymbolResolver().getResolvedFunctionSymbols()
            )) {
            sapphire::bootloader::RuntimeLinker::forceDllMainToFail(
                runtimeLinker, (HMODULE)NotificationData->Loaded.DllBase
            );
        }
    }
}

sapphire::bootloader::RuntimeLinker::RuntimeLinker(const sapphire::bootloader::SymbolResolver &resolver, ipc::Client &IPCClient) :
    mResolver(resolver),
    mIPCClient(IPCClient),
    mIatPatcher(std::make_unique<IatPatcher>("sapphire_bootloader.dll", IPCClient)) {
    if (MH_Initialize() != MH_OK) return;
    HMODULE hNtDll = GetModuleHandleW(L"ntdll.dll");
    auto    pLdrRegisterDllNotification =
        (PLDR_REGISTER_DLL_NOTIFICATION)GetProcAddress(hNtDll, "LdrRegisterDllNotification");

    if (pLdrRegisterDllNotification) {
        PVOID cookie;
        auto  res = pLdrRegisterDllNotification(0, LdrDllNotification, this, &cookie);
    }
}

sapphire::bootloader::RuntimeLinker::~RuntimeLinker() {
    MH_Uninitialize();
}