#include "RuntimeLinker.h"
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

VOID CALLBACK LdrDllNotification(
    ULONG                       NotificationReason,
    PCLDR_DLL_NOTIFICATION_DATA NotificationData,
    PVOID                       Context
) {
    if (NotificationReason == LDR_DLL_NOTIFICATION_REASON_LOADED) {
        auto runtimeLinker = static_cast<sapphire::bootloader::RuntimeLinker *>(Context);
        if (wcsstr(NotificationData->Loaded.BaseDllName->Buffer, L"sapphire_core_for_v1_21_50.dll")) {
            runtimeLinker->getIatPatcher().patchModule(
                (HMODULE)NotificationData->Loaded.DllBase, runtimeLinker->getSymbolResolver().getResolvedSymbols()
            );
        }
    }
}

sapphire::bootloader::RuntimeLinker::RuntimeLinker(const sapphire::bootloader::SymbolResolver &resolver) :
    mResolver(resolver), mIatPatcher(std::make_unique<IatPatcher>("sapphire_bootloader.dll")) {
    HMODULE hNtDll = GetModuleHandleW(L"ntdll.dll");
    auto    pLdrRegisterDllNotification =
        (PLDR_REGISTER_DLL_NOTIFICATION)GetProcAddress(hNtDll, "LdrRegisterDllNotification");

    if (pLdrRegisterDllNotification) {
        PVOID cookie;
        auto  res = pLdrRegisterDllNotification(0, LdrDllNotification, this, &cookie);
        InfoBox(L"bootloader, {:#x}", res);
    }
}

sapphire::bootloader::RuntimeLinker::~RuntimeLinker() {}