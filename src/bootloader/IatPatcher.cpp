#include "IatPatcher.h"
#include <delayimp.h>
#include <filesystem>
#include <format>

namespace sapphire::bootloader {

    IatPatcher::IatPatcher(const std::string &bedrockSigSourceDllName, ipc::Client &IPCClient) :
        mBedrockSigSourceDllName(bedrockSigSourceDllName), mIPCClient(IPCClient) {
    }

    bool IatPatcher::patchModule(HMODULE hModule, const ApiMap &apiMap, const ApiMap &dataApiMap) {
        return patchModuleInternal(hModule, apiMap, dataApiMap);
    }

    bool IatPatcher::patchModuleInternal(HMODULE hModuleToPatch, const ApiMap &apiMap, const ApiMap &dataApiMap) {
        if (hModuleToPatch == nullptr || mBedrockSigSourceDllName.empty()) {
            return false;
        }

        auto pImageBase = reinterpret_cast<PBYTE>(hModuleToPatch);
        auto pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(pImageBase);
        if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) return false;

        auto pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(pImageBase + pDosHeader->e_lfanew);
        if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE) return false;

        IMAGE_DATA_DIRECTORY &importDirectory = pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT];
        if (importDirectory.VirtualAddress == 0 || importDirectory.Size == 0) return true;

        auto pImportDesc = reinterpret_cast<PImgDelayDescr>(pImageBase + importDirectory.VirtualAddress);

        for (; pImportDesc->rvaDLLName != 0; ++pImportDesc) {
            char *pDllName = reinterpret_cast<char *>(pImageBase + pImportDesc->rvaDLLName);

            if (pDllName != mBedrockSigSourceDllName) continue;

            auto pINT = reinterpret_cast<PIMAGE_THUNK_DATA>(pImageBase + pImportDesc->rvaINT);
            auto pIAT = reinterpret_cast<PIMAGE_THUNK_DATA>(pImageBase + pImportDesc->rvaIAT);

            for (; pINT->u1.AddressOfData != 0; ++pINT, ++pIAT) {
                if (IMAGE_SNAP_BY_ORDINAL(pINT->u1.Ordinal)) continue;
                auto pImportByName =
                    reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(pImageBase + pINT->u1.AddressOfData);
                const char *importName = pImportByName->Name;
                auto        it = apiMap.find(importName);
                if (it == apiMap.end()) {
                    it = dataApiMap.find(importName);
                    if (it == dataApiMap.end()) {
                        wchar_t szPath[MAX_PATH];
                        if (GetModuleFileNameW(hModuleToPatch, szPath, MAX_PATH) != 0) {
                            std::filesystem::path path{szPath};
                            mIPCClient.send(
                                ipc::status::Error,
                                std::format("undefined symbol {}, referenced by {}", importName, path.stem().string())
                            );
                        } else {
                            mIPCClient.send(
                                ipc::status::Error,
                                std::format("undefined symbol {}, referenced by UNKNOWN", importName)
                            );
                        }
                        return false;
                    }
                }

                auto  realAddress = it->second;
                DWORD oldProtect;
                if (VirtualProtect(&pIAT->u1.Function, sizeof(void *), PAGE_READWRITE, &oldProtect)) {
                    pIAT->u1.Function = static_cast<ULONGLONG>(realAddress);
                    VirtualProtect(&pIAT->u1.Function, sizeof(void *), oldProtect, &oldProtect);
                }
            }
        }
        return true;
    }

} // namespace sapphire::bootloader