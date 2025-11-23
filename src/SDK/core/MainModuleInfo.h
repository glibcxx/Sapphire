#pragma once

#include "SDK/api/sapphire/logger/Logger.h"

#include <Psapi.h>
#include <winnt.h>

namespace sapphire {

    class MainModuleInfo {
    public:
        HMODULE    mMainModule = GetModuleHandleW(nullptr);
        MODULEINFO mMainModuleInfo;
        uintptr_t  mTextSectionStart = 0;
        size_t     mTextSectionSize = 0;

        MainModuleInfo() {
            GetModuleInformation(GetCurrentProcess(), this->mMainModule, &mMainModuleInfo, sizeof(MODULEINFO));
            uintptr_t moduleBase = reinterpret_cast<uintptr_t>(mMainModule);

            if (!moduleBase) {
                sapphire::error("MainModuleInfo: Failed to get main module handle.");
                return;
            }

            PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(moduleBase);
            if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
                sapphire::error("MainModuleInfo: Invalid DOS signature for main module.");
                return;
            }

            PIMAGE_NT_HEADERS ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(moduleBase + dosHeader->e_lfanew);
            if (ntHeaders->Signature != IMAGE_NT_SIGNATURE) {
                sapphire::error("MainModuleInfo: Invalid NT signature for main module.");
                return;
            }

            PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(ntHeaders);
            bool                  foundTextSection = false;
            for (WORD i = 0; i < ntHeaders->FileHeader.NumberOfSections; ++i, ++section) {
                if (memcmp(section->Name, ".text", 5) == 0 && section->Name[5] == '\0') {
                    mTextSectionStart = moduleBase + section->VirtualAddress;
                    mTextSectionSize = section->Misc.VirtualSize;
                    if (mTextSectionSize == 0) {
                        mTextSectionSize = section->SizeOfRawData;
                    }
                    foundTextSection = true;
                    sapphire::debug(
                        "MainModuleInfo: Found .text section in main module: VA=0x{:X}, Address=0x{:X}, Size=0x{:X}",
                        section->VirtualAddress,
                        mTextSectionStart,
                        mTextSectionSize
                    );
                    break;
                }
            }

            if (!foundTextSection || mTextSectionStart == 0 || mTextSectionSize == 0) {
                sapphire::error("MainModuleInfo: .text section not found or invalid in main module. Signature scanning for main module will fail.");
                mTextSectionStart = 0; // Ensure these are zero if not found
                mTextSectionSize = 0;
            }
        }
    };

} // namespace sapphire