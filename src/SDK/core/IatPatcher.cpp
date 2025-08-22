#include "IatPatcher.h"
#include "SDK/api/sapphire/logger/Logger.h"
#include "PluginManager.h"

namespace sapphire {

    IatPatcher &IatPatcher::getInstance() {
        static IatPatcher instance;
        return instance;
    }

    IatPatcher::IatPatcher() {
        HMODULE hSelf = nullptr;
        GetModuleHandleExA(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCSTR)&IatPatcher::getInstance,
            &hSelf
        );

        char szSdkPath[MAX_PATH];
        GetModuleFileNameA(hSelf, szSdkPath, MAX_PATH);

        const char *pFileName = strrchr(szSdkPath, '\\');
        if (pFileName) {
            mDllNames.emplace_back(pFileName + 1);
            Logger::Debug("[IatPatcher] Patcher initialized. SDK module name: {}", pFileName + 1);
        }
    }

    void IatPatcher::patchModule(HMODULE hModule, const ApiMap &apiMap) {
        patchModuleInternal(hModule, apiMap);
        char modulePath[MAX_PATH];
        GetModuleFileNameA(hModule, modulePath, MAX_PATH);
        const char *pFileName = strrchr(modulePath, '\\');
        if (pFileName) {
            mDllNames.emplace_back(pFileName + 1);
        }
    }

    bool IatPatcher::patchModuleInternal(HMODULE hModuleToPatch, const ApiMap &apiMap) {
        if (hModuleToPatch == nullptr || mDllNames.empty()) {
            return false;
        }

        auto pImageBase = reinterpret_cast<PBYTE>(hModuleToPatch);
        auto pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(pImageBase);
        if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) return false;

        auto pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(pImageBase + pDosHeader->e_lfanew);
        if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE) return false;

        Logger::Debug("[IatPatcher] Patching module at '{}'", (void *)pImageBase);

        IMAGE_DATA_DIRECTORY &importDirectory = pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
        if (importDirectory.VirtualAddress == 0) return true;

        auto pImportDesc = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(pImageBase + importDirectory.VirtualAddress);

        for (; pImportDesc->Name != 0; ++pImportDesc) {
            char *pDllName = reinterpret_cast<char *>(pImageBase + pImportDesc->Name);

            auto found = std::find_if(
                mDllNames.begin(), mDllNames.end(), [&](const std::string &dllName) { return dllName == pDllName; }
            );

            if (found == mDllNames.end())
                continue;
            char szModName[MAX_PATH];
            GetModuleFileNameA(hModuleToPatch, szModName, MAX_PATH);

            auto pOriginalThunk =
                reinterpret_cast<PIMAGE_THUNK_DATA>(pImageBase + pImportDesc->OriginalFirstThunk);
            auto pIAT = reinterpret_cast<PIMAGE_THUNK_DATA>(pImageBase + pImportDesc->FirstThunk);

            for (; pOriginalThunk->u1.AddressOfData != 0; ++pOriginalThunk, ++pIAT) {
                if (IMAGE_SNAP_BY_ORDINAL(pOriginalThunk->u1.Ordinal)) continue;

                auto pImportByName =
                    reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(pImageBase + pOriginalThunk->u1.AddressOfData);
                const char *functionName = pImportByName->Name;

                auto it = apiMap.find(functionName);
                if (it != apiMap.end()) {
                    auto realAddress = it->second;
                    Logger::Debug(
                        "[IatPatcher]  -> Redirecting function '{}' to address {}", functionName, realAddress
                    );

                    DWORD oldProtect;
                    if (VirtualProtect(&pIAT->u1.Function, sizeof(void *), PAGE_READWRITE, &oldProtect)) {
                        pIAT->u1.Function = static_cast<ULONGLONG>(realAddress);
                        VirtualProtect(&pIAT->u1.Function, sizeof(void *), oldProtect, &oldProtect);
                    }
                } else if (SPHR_DEBUG) {
                    std::string_view n{functionName};
                    if (n.find("sapphire@@") == std::string_view::npos
                        && n.find("ImGui@@") == std::string_view::npos)
                        Logger::Debug("[IatPatcher]  -> function '{}' not found in api map", functionName);
                }
            }
        }
        return true;
    }

} // namespace sapphire