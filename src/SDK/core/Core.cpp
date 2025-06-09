#include "Core.h"

#include <unordered_map>
#include <thread>

#include "SDK/api/sapphire/hook/Hook.h"
#include "util/time.h"

#include "DX12Hook.h"
#include "SDK/api/sapphire/util/DrawUtils.h"
#include "SDK/api/sapphire/config/Config.h"
#include "SDK/api/sapphire/event/events/eventImpls/EventHooks.h"
#include "SDK/api/sapphire/service/Service.h"

#include <Psapi.h> // for MODULEINFO
#include <winnt.h> // For PE header structures

namespace fs = std::filesystem;

namespace moduleInfo {
    HWND     gMainWindow = nullptr;
    uint64_t gStartTime = 0;
} // namespace moduleInfo

// 扫描特征码
uintptr_t ScanSignature(uintptr_t start, size_t size, const char *sig, size_t sigLength) {
    const char *data = reinterpret_cast<const char *>(start);
    for (uintptr_t i = 0; i <= size - sigLength; ++i) {
        bool match = true;
        for (uintptr_t j = 0; j < sigLength; ++j) {
            if (sig[j] != 0x00 && data[i + j] != sig[j]) // 0x00表示通配符
            {
                match = false;
                break;
            }
        }
        if (match) return start + i;
    }
    return 0;
}

namespace core {
    class CoreInfo {
    public:
        HMODULE    mMainModule = GetModuleHandleW(nullptr);
        MODULEINFO mMainModuleInfo;
        uintptr_t  mTextSectionStart = 0;
        size_t     mTextSectionSize = 0;

        CoreInfo() {
            GetModuleInformation(GetCurrentProcess(), this->mMainModule, &mMainModuleInfo, sizeof(MODULEINFO));
            uintptr_t moduleBase = reinterpret_cast<uintptr_t>(mMainModule);

            if (!moduleBase) {
                Logger::Error("[CoreInfo] Failed to get main module handle.");
                return;
            }

            PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(moduleBase);
            if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
                Logger::Error("[CoreInfo] Invalid DOS signature for main module.");
                return;
            }

            PIMAGE_NT_HEADERS ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(moduleBase + dosHeader->e_lfanew);
            if (ntHeaders->Signature != IMAGE_NT_SIGNATURE) {
                Logger::Error("[CoreInfo] Invalid NT signature for main module.");
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
                    Logger::Debug("[CoreInfo] Found .text section in main module: Address=0x{:X}, Size=0x{:X}", mTextSectionStart, mTextSectionSize);
                    break;
                }
            }

            if (!foundTextSection || mTextSectionStart == 0 || mTextSectionSize == 0) {
                Logger::Error("[CoreInfo] .text section not found or invalid in main module. Signature scanning for main module will fail.");
                mTextSectionStart = 0; // Ensure these are zero if not found
                mTextSectionSize = 0;
            }
        }

        static CoreInfo &getInstance() {
            static CoreInfo instance;
            return instance;
        }
    };

    SapphireModuleInfo::SapphireModuleInfo() :
        sapphireModuleHandle(GetModuleHandleW(L"sapphire_core.dll")) {
        GetModuleInformation(GetCurrentProcess(), this->sapphireModuleHandle, &sapphireModuleInfo, sizeof(MODULEINFO));
        wchar_t modulePathBuf[MAX_PATH] = {0};
        if (GetModuleFileNameW(sapphireModuleHandle, modulePathBuf, MAX_PATH) != 0) {
            sapphireHome = modulePathBuf;
            sapphireHome = sapphireHome.parent_path() / HOME_FOLDER_NAME;
        }
    }

    SapphireModuleInfo &getSapphireInfo() {
        static SapphireModuleInfo info{};
        return info;
    }

    uintptr_t getImagebase() {
        return reinterpret_cast<uintptr_t>(CoreInfo::getInstance().mMainModuleInfo.lpBaseOfDll);
    }

    uintptr_t scanApi(const char *sig, size_t sigLength) {
        /*
            目前api设计如下：
            SDK内头文件声明接口，而对应cpp文件实现为转发调用原始函数的指针。
            而指针在静态初始化期间完成扫描，所有函数接口的特征码直接写在对应函数体内。
        */
        CoreInfo &coreInfo = CoreInfo::getInstance();
        /*
            coreInfo用于保存主模块句柄，主模块大小等信息。
            不写成全局变量的原因是，不同编译单元的静态存储期对象初始化顺序是未知的，
            而api库创建接口就是完成于静态初始化期间，此时访问core的全局变量是未定义行为。
        */
        return ScanSignature(
            reinterpret_cast<uintptr_t>(coreInfo.mMainModule),
            coreInfo.mMainModuleInfo.SizeOfImage,
            sig,
            sigLength
        );
    }

} // namespace core

// 整个库的入口
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH: {
        winrt::init_apartment(winrt::apartment_type::single_threaded);
        DisableThreadLibraryCalls(hModule);
        moduleInfo::gStartTime = util::getTimeMs();
        moduleInfo::gMainWindow = FindWindow(0, L"Minecraft");
        if (!moduleInfo::gMainWindow) {
            Logger::Error("[core] 未找到 Minecraft 窗口！");
            Logger::ErrorBox(L"未找到 Minecraft 窗口！");
            winrt::uninit_apartment();
            return false;
        }
        std::thread{[]() {
            if (!DX12Hook::install()) {
                Logger::Error("[core] DX12 Hook 安装失败！");
                Logger::ErrorBox(L"DX12 Hook 安装失败！");
            }
        }}.detach();
        DrawUtils::getInstance();
        EventHooks::init();
        sapphire::service::init();
        break;
    }
    case DLL_PROCESS_DETACH:
        sapphire::service::uninit();
        EventHooks::uninit();
        DX12Hook::uninstall();
        winrt::uninit_apartment();
        break;
    default:
        break;
    }
    return TRUE;
}