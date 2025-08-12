#include <iostream>
#include <filesystem>
#include <regex>
#include <map>

#include "SDK/api/sapphire/logger/LogBox.hpp"
#include "util/ScopeGuard.hpp"

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Management.Deployment.h>
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.Foundation.Collections.h>

#include <AccCtrl.h>
#include <atlcomcli.h>
#include <aclapi.h>
#include <sddl.h>     // for ConvertStringSidToSid
#include <appmodel.h> // for FindPackagesByPackageFamily
#include <shobjidl.h> // for IApplicationActivationManager

#if WINVER < 0x0A00
#    error "please upgrade Windows SDK version!!"
#endif

using namespace winrt;
using namespace Windows::Management::Deployment;
using namespace Windows::ApplicationModel;
namespace fs = std::filesystem;
using namespace Logger;

typedef LONG(NTAPI *NtResumeProcess_t)(HANDLE ProcessHandle);
NtResumeProcess_t NtResumeProcess = (NtResumeProcess_t)GetProcAddress(GetModuleHandle(TEXT("ntdll")), "NtResumeProcess");

// https://www.unknowncheats.me/forum/general-programming-and-reversing/177183-basic-intermediate-techniques-uwp-app-modding.html
DWORD SetPermissions(const std::wstring &wstrFilePath, DWORD permission = GENERIC_READ | GENERIC_EXECUTE) {
    PACL                 pOldDACL = NULL, pNewDACL = NULL;
    PSECURITY_DESCRIPTOR pSD = NULL;
    EXPLICIT_ACCESS      eaAccess;
    SECURITY_INFORMATION siInfo = DACL_SECURITY_INFORMATION;
    DWORD                dwResult = ERROR_SUCCESS;
    PSID                 pSID;

    // Get a pointer to the existing DACL
    dwResult = GetNamedSecurityInfo(wstrFilePath.c_str(), SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &pOldDACL, NULL, &pSD);
    if (dwResult != ERROR_SUCCESS)
        goto Cleanup;

    // Get the SID for ALL APPLICATION PACKAGES using its SID string
    ConvertStringSidToSid(L"S-1-15-2-1", &pSID);
    if (pSID == NULL)
        goto Cleanup;

    ZeroMemory(&eaAccess, sizeof(EXPLICIT_ACCESS));
    eaAccess.grfAccessPermissions = permission;
    eaAccess.grfAccessMode = SET_ACCESS;
    eaAccess.grfInheritance = SUB_CONTAINERS_AND_OBJECTS_INHERIT;
    eaAccess.Trustee.TrusteeForm = TRUSTEE_IS_SID;
    eaAccess.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    eaAccess.Trustee.ptstrName = (LPWSTR)pSID;

    // Create a new ACL that merges the new ACE into the existing DACL
    dwResult = SetEntriesInAcl(1, &eaAccess, pOldDACL, &pNewDACL);
    if (ERROR_SUCCESS != dwResult)
        goto Cleanup;

    // Attach the new ACL as the object's DACL
    dwResult = SetNamedSecurityInfo((LPWSTR)wstrFilePath.c_str(), SE_FILE_OBJECT, siInfo, NULL, NULL, pNewDACL, NULL);
    if (ERROR_SUCCESS != dwResult)
        goto Cleanup;

Cleanup:
    if (pSD != NULL)
        LocalFree((HLOCAL)pSD);
    if (pNewDACL != NULL)
        LocalFree((HLOCAL)pNewDACL);

    return dwResult;
}

std::wstring PackageFullNameFromFamilyName(const std::wstring &familyName) {
    UINT32 count = 0, length = 0;
    LONG   status = FindPackagesByPackageFamily(familyName.c_str(), PACKAGE_FILTER_HEAD, &count, nullptr, &length, nullptr, nullptr);
    if (status == ERROR_SUCCESS || status != ERROR_INSUFFICIENT_BUFFER)
        return {};

    std::wstring buffer(length * count, L'\0');
    wchar_t     *fullname = nullptr;
    status = FindPackagesByPackageFamily(familyName.c_str(), PACKAGE_FILTER_HEAD, &count, &fullname, &length, buffer.data(), nullptr);
    if (status == ERROR_SUCCESS)
        return fullname;

    return {};
}

DWORD launchUWP(const std::wstring &appUserModelId) {
    CComPtr<IApplicationActivationManager> manager;

    HRESULT result = CoCreateInstance(
        CLSID_ApplicationActivationManager,
        NULL,
        CLSCTX_LOCAL_SERVER,
        IID_IApplicationActivationManager,
        (LPVOID *)&manager
    );
    if (!SUCCEEDED(result)) return result;

    result = CoAllowSetForegroundWindow(manager, NULL);
    if (!SUCCEEDED(result)) return result;

    DWORD dwProcessId = 0;
    manager->ActivateApplication(appUserModelId.data(), NULL, AO_NONE, &dwProcessId);
    return dwProcessId;
}

DWORD launchAndAttachToDebugger(const std::wstring &familyName, const fs::path &debuggerPath) {
    std::wstring appFullName = PackageFullNameFromFamilyName(familyName);
    if (appFullName.empty())
        return 0;

    ATL::CComQIPtr<IPackageDebugSettings> debugSettings;

    HRESULT hResult = debugSettings.CoCreateInstance(CLSID_PackageDebugSettings, NULL, CLSCTX_ALL);
    if (hResult != S_OK) return 0;

    hResult = debugSettings->EnableDebugging(appFullName.c_str(), debuggerPath.c_str(), NULL);
    if (hResult != S_OK) return 0;

    DWORD pid = launchUWP(familyName + L"!App");

    debugSettings->DisableDebugging(appFullName.c_str());
    return pid;
}

void disableDebugging(const std::wstring &familyName) {
    std::wstring appFullName = PackageFullNameFromFamilyName(familyName);
    if (appFullName.empty())
        return;

    ATL::CComQIPtr<IPackageDebugSettings> debugSettings;

    HRESULT hResult = debugSettings.CoCreateInstance(CLSID_PackageDebugSettings, NULL, CLSCTX_ALL);
    if (hResult != S_OK) return;

    debugSettings->DisableDebugging(appFullName.c_str());
}

using AutoCloseHandle = std::unique_ptr<std::remove_pointer_t<HANDLE>, decltype([](HANDLE h) { CloseHandle(h); })>;
using AutoVirtualFree = std::unique_ptr<std::remove_pointer_t<LPVOID>, decltype([](LPVOID m) { VirtualFree(m, 0, MEM_RELEASE); })>;

DWORD injectDll(HANDLE hProcess, const fs::path &dllPath) {
    std::wstring dllPathStr = dllPath;
    SetPermissions(dllPath);

    // 在目标进程分配内存
    AutoVirtualFree pRemotePath{VirtualAllocEx(
        hProcess,
        nullptr,
        (dllPathStr.size() + 1) * sizeof(wchar_t),
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    )};
    if (!pRemotePath) {
        Logger::ErrorBox(L"[injector][{}]\n内存分配失败 (错误码: {})", dllPath.filename().c_str(), GetLastError());
        return false;
    }

    // 写入DLL路径
    if (!WriteProcessMemory(
            hProcess,
            pRemotePath.get(),
            dllPathStr.c_str(),
            (dllPathStr.size() + 1) * sizeof(wchar_t),
            nullptr
        )) {
        Logger::ErrorBox(L"[injector][{}]\n写入内存失败 (错误码: {})", dllPath.filename().c_str(), GetLastError());
        return false;
    }

    // 创建远程线程调用LoadLibraryW
    AutoCloseHandle hThread{
        CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)LoadLibraryW, pRemotePath.get(), 0, nullptr),
    };
    if (!hThread) {
        Logger::ErrorBox(L"[injector][{}]\n创建远程线程失败 (错误码: {})", dllPath.filename().c_str(), GetLastError());
        return false;
    }
    // 等待线程结束
    WaitForSingleObject(hThread.get(), INFINITE);

    DWORD exitCode;
    GetExitCodeThread(hThread.get(), &exitCode);
    if (exitCode == STILL_ACTIVE)
        Logger::WarnBox(L"[injector][{}]\n远程线程尚未退出，不能安全释放内存", dllPath.filename().c_str());

    return exitCode;
}

fs::path getCurrentPath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    return fs::path{buffer}.parent_path();
}

struct VersionInfo {
    uint16_t Major;
    uint16_t Minor;
    uint16_t Build;
    uint16_t Revision;

    constexpr VersionInfo(uint16_t ma = 0, uint16_t mi = 0, uint16_t b = 0, uint16_t r = 0) :
        Major{ma}, Minor{mi}, Build{b}, Revision{r} {}
    constexpr VersionInfo(const PackageVersion &v) :
        Major{v.Major}, Minor{v.Minor}, Build{v.Build}, Revision{v.Revision} {}

    constexpr std::strong_ordering operator<=>(const VersionInfo &rhs) const = default;
};

std::optional<VersionInfo> getMinecraftVersion() {
    try {
        PackageManager packageManager;
        for (auto &&package : packageManager.FindPackagesForUser(L"", L"Microsoft.MinecraftUWP_8wekyb3d8bbwe")) {
            return package.Id().Version();
        }
    } catch (const winrt::hresult_error &ex) {
        std::wcerr << L"Error: " << ex.message().c_str() << std::endl;
    }
    return std::nullopt;
}

std::filesystem::path getBestCompatibleVersion(const VersionInfo &uwpInternalVersion, const std::filesystem::path &path) {
    std::map<VersionInfo, std::filesystem::path, std::greater<VersionInfo>> versions;
    Logger::InfoBox(L"Searching begin");
    for (auto &&entry : fs::directory_iterator{path}) {
        if (!entry.is_regular_file())
            continue;
        auto        filename = entry.path().filename().string();
        std::regex  reg{"sapphire_core_for_v(\\d+)_(\\d+)_(\\d+)\\.dll"};
        std::smatch matched;
        if (!std::regex_match(filename, matched, reg))
            continue;
        VersionInfo ver{
            (uint16_t)std::stoi(matched[1].str()),
            (uint16_t)std::stoi(matched[2].str()),
            (uint16_t)std::stoi(matched[3].str())
        };
        versions.emplace(ver, entry.path());
    }
    if (versions.empty())
        return {};
    VersionInfo serverSideVersion{
        uwpInternalVersion.Major,
        uwpInternalVersion.Minor,
        (uint16_t)(uwpInternalVersion.Build / 100),
        (uint16_t)(uwpInternalVersion.Build % 100)
    };
    auto it = versions.lower_bound(serverSideVersion);
    if (it == versions.end())
        return {};
    return it->second;
}

int main(int argc, char **argv) {
    /*
        UWP的启动方式与普通的Win32程序不同，这里的思路来自上面那个链接里的帖子。
        基本流程如下：
            1. 通过familyName获取appFullName，前者可以表示一个应用，后者则还会区分版本
            2. 利用IPackageDebugSettings为目标应用启用debug模式，需要appFullName，
            同时指定一个调试器，而我们的调试器就是本 UWPinjecter.exe。
            3. 通过IApplicationActivationManager::ActivateApplication启动目标应用，需要familyName加上"!App"后缀，
            此时游戏以附加调试器启动，并挂起。
            4. 另一个UWPinjecter.exe进程被启动，并带有一个-p参数指定游戏的pid。游戏进程已经挂起，
            只需要用常规方法注入dll即可(但是需要dll有被执行权限，这里用代码修改)
            5. 最后，调试器内恢复游戏进程，并关闭debug即可
    */

    HRESULT hResult = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hResult)) {
        Logger::ErrorBox(L"[injector] CoInitializeEx 失败 (错误码: {})", hResult);
        return 1;
    }

    DWORD dwProcessId = 0;
    for (int i = 1; i < argc; i += 2) {
        std::string arg(argv[i]);
        if (arg == "-p")
            dwProcessId = atoi(argv[i + 1]);
    }

    fs::path currentDir = getCurrentPath();
    if (dwProcessId == 0) {
        launchAndAttachToDebugger(L"Microsoft.MinecraftUWP_8wekyb3d8bbwe", currentDir / L"sapphire_launcher.exe");
    } else {
        bool             injectionSuccessful = false;
        util::ScopeGuard autoTerminateOrResume{
            [dwProcessId, &injectionSuccessful]() {
                disableDebugging(L"Microsoft.MinecraftUWP_8wekyb3d8bbwe");
                HANDLE hTargetProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
                if (hTargetProcess && hTargetProcess != INVALID_HANDLE_VALUE) {
                    if (injectionSuccessful) {
                        NtResumeProcess(hTargetProcess);
                    } else {
                        TerminateProcess(hTargetProcess, 1);
                    }
                    CloseHandle(hTargetProcess);
                }
            }
        };
        auto mcVersion = getMinecraftVersion();
        if (!mcVersion) {
            Logger::ErrorBox(L"[injector] 无法获取游戏版本信息。");
            return 1;
        }
        auto dllPath = getBestCompatibleVersion(*mcVersion, currentDir);
        if (dllPath.empty()) {
            Logger::ErrorBox(
                L"[injector] 无法找到一个兼容游戏的 Sapphire 版本。游戏版本：{}.{}.{}.{}",
                mcVersion->Major,
                mcVersion->Minor,
                mcVersion->Build,
                mcVersion->Revision
            );
            return 1;
        }

        if (!fs::exists(dllPath)) {
            currentDir = fs::path{argv[0]}.parent_path();
            dllPath = currentDir / L"sapphire_core.dll";
            if (!fs::exists(dllPath)) {
                Logger::ErrorBox(L"[injector] 未找到dll内核：{}", dllPath.c_str());
                return 1;
            }
        }
        fs::path homePath = currentDir / L"sapphire";
        if (!std::filesystem::exists(homePath))
            std::filesystem::create_directories(homePath);
        fs::path modsPath = homePath / "mods";
        if (!std::filesystem::exists(modsPath))
            std::filesystem::create_directories(modsPath);
        SetPermissions(homePath, GENERIC_READ | GENERIC_WRITE);
        SetPermissions(modsPath, GENERIC_READ | GENERIC_EXECUTE);
        AutoCloseHandle hProcess{OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId)};
        if (!hProcess && hProcess.get() == INVALID_HANDLE_VALUE) {
            Logger::ErrorBox(L"[injector][{}]\n打开进程失败 (错误码: {})", dllPath.filename().c_str(), GetLastError());
            return 1;
        }

        PSECURITY_DESCRIPTOR p_sd = nullptr;
        SECURITY_ATTRIBUTES  sa = {};
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle = FALSE;
        ConvertStringSecurityDescriptorToSecurityDescriptorW(
            L"D:(A;;GA;;;WD)(A;;GA;;;AC)",
            SDDL_REVISION_1,
            &p_sd,
            nullptr
        );
        sa.lpSecurityDescriptor = p_sd;
        std::wstring    pipe_name = L"\\\\.\\pipe\\SapphireSignalPipe";
        AutoCloseHandle h_pipe{CreateNamedPipe(
            pipe_name.c_str(),
            PIPE_ACCESS_INBOUND,
            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
            1,
            1024,
            1024,
            0,
            &sa
        )};

        if (p_sd) LocalFree(p_sd);
        if (h_pipe.get() == INVALID_HANDLE_VALUE) {
            Logger::ErrorBox(L"[injector] 无法打开通信管道 (错误码: {})", GetLastError());
            return 1;
        }

        DWORD code = injectDll(hProcess.get(), dllPath);
        if (code == 0) {
            Logger::ErrorBox(L"[injector] sapphire_core.dll 注入失败！");
            return 1;
        }
        BOOL b_connected = ConnectNamedPipe(h_pipe.get(), nullptr);
        if (!b_connected) {
            if (GetLastError() != ERROR_PIPE_CONNECTED) {
                Logger::ErrorBox(L"[injector] 连接通信管道失败 (错误码: {})", GetLastError());
                return 1;
            }
        }

        char  buffer[256] = {0};
        DWORD bytes_read = 0;
        BOOL  b_result = ReadFile(h_pipe.get(), buffer, sizeof(buffer) - 1, &bytes_read, nullptr);
        if (b_result && bytes_read > 0) {
            if (strcmp(buffer, "READY") == 0) {
                std::cout << "'Ready' signal received.\n";
                injectionSuccessful = true;
            } else {
                return 1;
            }
        } else {
            Logger::ErrorBox(L"[injector] 读取通信管道失败 (错误码: {})", GetLastError());
            return 1;
        }
    }
    CoUninitialize();
    return S_OK;
}