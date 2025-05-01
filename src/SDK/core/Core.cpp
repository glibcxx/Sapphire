#include "Core.h"

#include <unordered_map>

#include "util/time.h"

#include <Psapi.h> // for MODULEINFO

namespace fs = std::filesystem;

static std::unordered_map<uintptr_t, uintptr_t> *gApiToOriginMap = nullptr;

namespace moduleInfo {
    HWND     gMainWindow = nullptr;
    uint64_t gStartTime = 0;

} // namespace moduleInfo

// 扫描特征码
uintptr_t ScanSignature(uintptr_t start, size_t size, const char *sig, size_t sigLength) {
    const char *data = reinterpret_cast<const char *>(start);
    for (uintptr_t i = 0; i < size - sigLength; ++i) {
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

        std::unordered_map<uintptr_t, uintptr_t> ApiAddrToOriginAddr;
        CoreInfo() {
            gApiToOriginMap = &ApiAddrToOriginAddr;
            GetModuleInformation(GetCurrentProcess(), this->mMainModule, &mMainModuleInfo, sizeof(MODULEINFO));
        }

        static CoreInfo &getInstance() {
            static CoreInfo instance;
            return instance;
        }
    };

    SDK_API uintptr_t getImagebase() {
        return reinterpret_cast<uintptr_t>(CoreInfo::getInstance().mMainModuleInfo.lpBaseOfDll);
    }

    uintptr_t getOrigin(uintptr_t api) {
        if (!gApiToOriginMap) return 0;
        if (auto it = gApiToOriginMap->find(api); it != gApiToOriginMap->end())
            return it->second;
        else
            return 0;
    }

    uintptr_t registryApi(const char *sig, size_t sigLength, uintptr_t api) {
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
        uintptr_t res = ScanSignature(
            reinterpret_cast<uintptr_t>(coreInfo.mMainModule),
            coreInfo.mMainModuleInfo.SizeOfImage,
            sig,
            sigLength
        );
        if (api)
            coreInfo.ApiAddrToOriginAddr.emplace(api, res);
        return res;
    }

} // namespace core

// 整个库的入口
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hModule);
        hook::init();
        moduleInfo::gStartTime = util::getTimeMs();
        moduleInfo::gMainWindow = FindWindow(0, L"Minecraft");
        if (!moduleInfo::gMainWindow) {
            Logger::Error("未找到 Minecraft 窗口！");
            return false;
        }
        break;
    }
    case DLL_PROCESS_DETACH:
        hook::uninit();
        FreeLibraryAndExitThread(hModule, TRUE);
        break;
    default:
        break;
    }
    return TRUE;
}