#include "LauncherApp.h"
#include "DebuggerApp.h"
#include "common/IPC/Pipe.h"
#include "common/IPC/PipeChannel.h"
#include "common/IPC/Protocal.h"
#include "common/coroutine/AsyncScope.hpp"
#include "common/coroutine/IoContext.hpp"
#include "common/coroutine/SyncWait.hpp"
#include "common/coroutine/Task.hpp"
#include "common/coroutine/WhenAll.hpp"
#include "common/sys/MiniWindows.h"

#include <atlcomcli.h>
#include <aclapi.h>
#include <iostream>
#include <sddl.h>
#include <appmodel.h>
#include <shobjidl.h>
#include <processthreadsapi.h>
#include <stdexcept>
#include <winternl.h>

namespace fs = std::filesystem;

namespace sapphire::launcher {

    LauncherApp::LauncherApp() : mLauncherAppPath(MAX_PATH, {}) {
        HRESULT hResult = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        if (FAILED(hResult)) {
            std::cerr << std::format("[launcher] CoInitializeEx 失败 (code: {})\n", hResult);
            throw std::runtime_error{"[launcher] CoInitializeEx 失败"};
        }

        GetModuleFileNameW(nullptr, mLauncherAppPath.data(), mLauncherAppPath.size());
        mLauncherAppDir = fs::path{mLauncherAppPath}.parent_path();
    }

    LauncherApp::~LauncherApp() noexcept {
        if (mDebugSettings)
            mDebugSettings->DisableDebugging(mAppFullName.data());

        if (mGameProcessHandle && mGameProcessHandle != INVALID_HANDLE_VALUE) {
            typedef NTSTATUS(NTAPI * NtResumeProcess_t)(HANDLE ProcessHandle);
            NtResumeProcess_t NtResumeProcess = (NtResumeProcess_t)GetProcAddress(GetModuleHandle(TEXT("ntdll")), "NtResumeProcess");
            NTSTATUS          status = NtResumeProcess(mGameProcessHandle);
            if (!NT_SUCCESS(status)) {
                std::cout << std::format("[launcher] fail to resume game, code: {:#X}\n", status);
            }
            CloseHandle(mGameProcessHandle);
        }
        CoUninitialize();
    }

    int LauncherApp::run() {
        std::wstring familyName = L"Microsoft.MinecraftUWP_8wekyb3d8bbwe";
        mAppFullName = packageFullNameFromFamilyName(familyName);
        if (mAppFullName.empty()) {
            std::cerr << "[launcher] 未找到游戏，你可能未安装游戏\n";
            return -1;
        }

        HRESULT hResult = mDebugSettings.CoCreateInstance(CLSID_PackageDebugSettings, NULL, CLSCTX_ALL);
        if (hResult != S_OK) {
            std::cerr << std::format("[launcher] 创建 PackageDebugSettings 失败 (code: {})\n", hResult);
            return -1;
        }

        hResult = mDebugSettings->EnableDebugging(mAppFullName.data(), mLauncherAppPath.data(), NULL);
        if (hResult != S_OK) {
            std::cerr << std::format("[launcher] 无法配置调试器 (code: {})\n", hResult);
            return 0;
        }

        auto ctx = coro::IoContext::create(1);
        if (!ctx) {
            std::cerr << std::format("[launcher] 无法创建 IoContext, msg: {}\n", ctx.error().message());
            return -1;
        }
        auto debuggerPipe = ipc::backend::Pipe::create(L"\\\\.\\pipe\\SapphireSignalPipe.Debugger", *ctx);
        if (!debuggerPipe) {
            std::cerr << std::format("[launcher] 无法创建 Pipe, msg: {}\n", ctx.error().message());
            return -1;
        }

        syncWait(whenAll(
            launchGame(*ctx, familyName, std::move(debuggerPipe.value())),
            [](coro::IoContext &ctx) -> coro::Task<bool> {
                ctx.processEvents();
                co_return true;
            }(*ctx)
        ));

        return 0;
    }

    coro::Task<bool> LauncherApp::launchGame(
        coro::IoContext    &ctx,
        const std::wstring &familyName,
        ipc::backend::Pipe  debuggerPipe
    ) {
        ScopeGuard guard{[&]() { ctx.stop(); }};

        auto launchGameTask = [&]() -> coro::Task<void> {
            mLaunchThread = std::jthread{[familyName = familyName, this]() mutable {
                HRESULT hResult = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
                if (FAILED(hResult)) {
                    std::cerr << std::format("[launcher] CoInitializeEx 失败 (code: {})\n", hResult);
                    throw std::runtime_error{"[launcher] CoInitializeEx 失败"};
                }

                familyName.append(L"!App");
                Expected<DWORD, HRESULT> gamePid = this->launchUWP(familyName);

                if (gamePid.hasError()) {
                    std::cerr << std::format("[launcher] 无法启动游戏 (code: {:#X})\n", (unsigned long)gamePid.error());
                } else {
                    std::cout << std::format("[launcher] 游戏已正常运行 :)\n");
                }
                CoUninitialize();
            }};
            co_return;
        };

        auto [code, _] = co_await whenAll(debuggerPipe.listen(), launchGameTask());
        if (code.result()) {
            std::cerr << std::format("[launcher] 连接管道时出现错误, msg: {}\n", code.result().message());
            co_return false;
        }
        std::cout << "[launcher] 已连接到 Debugger 进程\n";

        sys::win::dword_t gamePid = 0;
        size_t            totalRead = 0;
        while (totalRead < sizeof(gamePid)) {
            auto read = co_await debuggerPipe.read((char *)&gamePid + totalRead, sizeof(gamePid) - totalRead);
            if (read.hasError()) {
                std::cerr << std::format("[launcher] 无法获取游戏 pid (msg: {})\n", read.error().message());
                co_return false;
            }
            totalRead += read.value();
        }
        mGameProcessId = gamePid;

        auto dllPath = mLauncherAppDir / "bin" / "sapphire_bootloader.dll";
        if (!fs::exists(dllPath)) {
            std::cerr << std::format("[launcher] 未找到 sapphire_bootloader：{}\n", dllPath.string());
            co_return false;
        }
        ensureDataDirExist();
        mGameProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, mGameProcessId);
        if (mGameProcessHandle == INVALID_HANDLE_VALUE) {
            std::cerr << std::format("[launcher] 无法获取游戏进程句柄 (pid: {}, code: {})\n", mGameProcessId, GetLastError());
            co_return false;
        }

        co_await whenAll(
            pipeServerMain(ctx),
            [](coro::IoContext &ctx, sys::win::handle_t hProcess, const std::filesystem::path &dllPath) -> coro::Task<int> {
                if (!injectDll(hProcess, dllPath)) {
                    ctx.stop();
                    co_return -1;
                }
                co_return 0;
            }(ctx, mGameProcessHandle, dllPath)
        );

        (void)co_await debuggerPipe.write("\x01", 1);

        if (mInjectSuccess) {
            using namespace std::chrono_literals;
            std::cout << "[launcher] Pipe disconnected!\n";
            std::cout << "[launcher] All task done.\n";
            co_return true;
        } else {
            std::cerr << "[launcher] Error occurred. See more info above.\n";
            co_return false;
        }

        co_return true;
    }

    coro::Task<int> LauncherApp::pipeServerMain(coro::IoContext &ctx) {
        ScopeGuard guard{[&]() { ctx.stop(); }};

        std::cout << "[launcher] Pipe connected!\n";

        coro::AsyncScope scope;

        auto optPipe = ipc::backend::Pipe::create(L"\\\\.\\pipe\\SapphireSignalPipe.Bootloader", ctx);
        if (!optPipe) {
            std::cerr << std::format("[launcher] 无法创建通信管道 (\\\\.\\pipe\\SapphireSignalPipe.Bootloader), msg: {}", optPipe.error().message());
            co_return -1;
        }
        auto optPipe2 = ipc::backend::Pipe::create(L"\\\\.\\pipe\\SapphireSignalPipe.Core", ctx);
        if (!optPipe2) {
            std::cerr << std::format("[launcher] 无法创建通信管道 (\\\\.\\pipe\\SapphireSignalPipe.Core), msg: {}", optPipe2.error().message());
            co_return -1;
        }
        scope.spawn(handlePipeConnection(ctx, std::move(*optPipe)));
        scope.spawn(handlePipeConnection(ctx, std::move(*optPipe2)));

        co_await scope.join();
        co_return 1;
    }

    coro::Task<int> LauncherApp::handlePipeConnection(
        coro::IoContext   &ctx,
        ipc::backend::Pipe pipe
    ) {
        ipc::PipeChannel channel{pipe};
        ipc::Message     msg;

        auto code = co_await pipe.listen();
        if (code) {
            std::cerr << "[launcher] Pipe connection failed!\n";
            co_return -1;
        }

        while (co_await channel.recv(msg)) {
            auto status = msg.getStatus();
            if (status == ipc::status::Handshake) {
                std::cout << "[launcher] Pipe connection from: " << msg.getData() << '\n';
            } else if (status == ipc::status::Handoff) {
                std::cout << "[launcher] Pipe disconnecting from: " << msg.getData() << '\n';
                if (msg.getData() == "Sapphire Core") {
                    mInjectSuccess = true;
                }
                break;
            } else if (status == ipc::status::Error) {
                std::cerr << "[launcher] Error msg recieved: " << msg.getData() << '\n';
                co_return -1;
            } else if (status == ipc::status::Success) {
                if (msg.getData().is_string())
                    std::cout << msg.getData().get<std::string>() << '\n';
                else
                    std::cout << "[launcher] Invalid msg type: " << msg.getData().type_name() << '\n';
            }
        }
        co_return 0;
    }

    void LauncherApp::ensureDataDirExist() {
        fs::path binPath = mLauncherAppDir / "bin";
        if (!std::filesystem::exists(binPath))
            std::filesystem::create_directories(binPath);
        fs::path homePath = mLauncherAppDir / "sapphire";
        if (!std::filesystem::exists(homePath))
            std::filesystem::create_directories(homePath);
        fs::path modsPath = homePath / "mods";
        if (!std::filesystem::exists(modsPath))
            std::filesystem::create_directories(modsPath);
        setPermissions(binPath, GENERIC_READ | GENERIC_EXECUTE);
        setPermissions(homePath, GENERIC_READ | GENERIC_WRITE);
        setPermissions(modsPath, GENERIC_READ | GENERIC_EXECUTE);
    }

    sys::win::dword_t LauncherApp::setPermissions(
        const std::wstring &wstrFilePath,
        sys::win::dword_t   permission = GENERIC_READ | GENERIC_EXECUTE
    ) {
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

    Expected<DWORD, HRESULT> LauncherApp::launchUWP(const std::wstring &appUserModelId) {
        CComPtr<IApplicationActivationManager> manager;

        HRESULT result = CoCreateInstance(
            CLSID_ApplicationActivationManager,
            NULL,
            CLSCTX_LOCAL_SERVER,
            IID_IApplicationActivationManager,
            (LPVOID *)&manager
        );
        if (!SUCCEEDED(result)) return {unexpected, result};

        result = CoAllowSetForegroundWindow(manager, NULL);
        if (!SUCCEEDED(result)) return {unexpected, result};

        DWORD dwProcessId = 0;
        result = manager->ActivateApplication(appUserModelId.data(), NULL, AO_NONE, &dwProcessId);
        if (!SUCCEEDED(result)) return {unexpected, result};

        return dwProcessId;
    }

    std::wstring LauncherApp::packageFullNameFromFamilyName(
        const std::wstring &familyName
    ) {
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

    bool LauncherApp::injectDll(
        sys::win::handle_t           hProcess,
        const std::filesystem::path &dllPath
    ) {
        using AutoCloseHandle = std::unique_ptr<std::remove_pointer_t<HANDLE>, decltype([](HANDLE h) { CloseHandle(h); })>;
        auto remoteFreeDeleter = [hProcess](LPVOID p) {
            if (p) VirtualFreeEx(hProcess, p, 0, MEM_RELEASE);
        };
        using AutoVirtualFree = std::unique_ptr<std::remove_pointer_t<LPVOID>, decltype(remoteFreeDeleter)>;

        std::wstring dllPathStr = dllPath;
        setPermissions(dllPath);

        fs::path pdbPath = dllPath;
        pdbPath.replace_extension(".pdb");
        if (fs::exists(pdbPath)) {
            setPermissions(pdbPath, GENERIC_READ);
        }

#pragma pack(push, 8)
        struct THREAD_PARAM {
            FARPROC pfnLoadLibraryW;     // 偏移 0x00
            FARPROC pfnGetLastError;     // 0x08
            wchar_t szDllPath[MAX_PATH]; // 0x10
        };
#pragma pack(pop)

        // clang-format off
        static const unsigned char gX64Stub[] = {
            0x53,                               // 0x00: push   rbx
            0x48, 0x83, 0xEC, 0x20,             // 0x01: sub    rsp, 0x20
            0x48, 0x89, 0xCB,                   // 0x05: mov    rbx, rcx
            0x48, 0x8D, 0x4B, 0x10,             // 0x08: lea    rcx, [rbx + 0x10]
            0xFF, 0x13,                         // 0x0C: call   qword ptr [rbx]
            0x48, 0x85, 0xC0,                   // 0x0E: test   rax, rax
            0x75, 0x05,                         // 0x11: jnz    +0x05 (准确跳向 0x18: xor eax, eax)
            0xFF, 0x53, 0x08,                   // 0x13: call   qword ptr [rbx + 0x8]
            0xEB, 0x02,                         // 0x16: jmp    +0x02 (跳向 0x1A: add rsp, 0x20)
            // success:
            0x31, 0xC0,                         // 0x18: xor    eax, eax
            // exit:
            0x48, 0x83, 0xC4, 0x20,             // 0x1A: add    rsp, 0x20
            0x5B,                               // 0x1E: pop    rbx
            0xC3                                // 0x1F: ret
        };
        // clang-format on

        THREAD_PARAM params = {};
        HMODULE      hK32 = GetModuleHandleW(L"kernel32.dll");
        params.pfnLoadLibraryW = GetProcAddress(hK32, "LoadLibraryW");
        params.pfnGetLastError = GetProcAddress(hK32, "GetLastError");
        wcsncpy_s(params.szDllPath, dllPath.c_str(), _TRUNCATE);

        SIZE_T codeSize = sizeof(gX64Stub);
        SIZE_T dataSize = sizeof(THREAD_PARAM);
        SIZE_T totalSize = codeSize + dataSize;

        AutoVirtualFree pRemoteBuf{VirtualAllocEx(hProcess, nullptr, totalSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE), remoteFreeDeleter};
        if (!pRemoteBuf) {
            std::cerr << std::format("[launcher][{}]\n内存分配失败 (code: {})\n", dllPath.filename().string(), GetLastError());
            return false;
        }

        LPVOID pRemoteCode = pRemoteBuf.get();
        LPVOID pRemoteData = (char *)pRemoteBuf.get() + codeSize;

        if (!WriteProcessMemory(hProcess, pRemoteCode, gX64Stub, codeSize, nullptr)
            || !WriteProcessMemory(hProcess, pRemoteData, &params, dataSize, nullptr)) {
            std::cerr << std::format("[launcher][{}]\n写入内存失败 (code: {})\n", dllPath.filename().string(), GetLastError());
            return false;
        }

        AutoCloseHandle hThread{
            CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)pRemoteCode, pRemoteData, 0, nullptr),
        };
        if (!hThread) {
            std::cerr << std::format("[launcher][{}]\n创建远程线程失败 (code: {})\n", dllPath.filename().string(), GetLastError());
            return false;
        }
        WaitForSingleObject(hThread.get(), INFINITE);

        DWORD remoteErrorCode;
        if (!GetExitCodeThread(hThread.get(), &remoteErrorCode)) {
            std::cerr << std::format("[launcher][{}]\n无法获取远程线程返回值 (code: {})\n", dllPath.filename().string(), GetLastError());
            return false;
        }

        if (remoteErrorCode != ERROR_SUCCESS) {
            std::cerr << std::format("[launcher][{}]\nDll注入失败 (code: {})\n", dllPath.filename().string(), remoteErrorCode);
            return false;
        }

        return true;
    }

    void LauncherApp::disableDebugging(const std::wstring &familyName) {
        std::wstring appFullName = LauncherApp::packageFullNameFromFamilyName(familyName);
        if (appFullName.empty())
            return;

        ATL::CComQIPtr<IPackageDebugSettings> debugSettings;

        HRESULT hResult = debugSettings.CoCreateInstance(CLSID_PackageDebugSettings, NULL, CLSCTX_ALL);
        if (FAILED(hResult)) {
            std::cerr << std::format("[Debugger] disableDebugging - CoCreateInstance 失败, code: 0x{:X}\n", (unsigned long)hResult);
            return;
        }

        HRESULT res = debugSettings->DisableDebugging(appFullName.c_str());
        if (FAILED(res)) {
            std::cerr << std::format("[Debugger] DisableDebugging 失败, code: 0x{:X}\n", (unsigned long)res);
        }
    }

} // namespace sapphire::launcher