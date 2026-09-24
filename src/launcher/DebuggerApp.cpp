#include "DebuggerApp.h"
#include "LauncherApp.h"
#include "Logger.h"
#include "common/coroutine/AsyncScope.hpp"
#include "common/coroutine/IoContext.hpp"
#include "common/coroutine/SyncWait.hpp"
#include "common/coroutine/Task.hpp"
#include "common/coroutine/WhenAll.hpp"
#include "common/IPC/Pipe.h"
#include "common/IPC/PipeChannel.h"
#include "common/IPC/Protocal.h"
#include "common/ScopeGuard.hpp"
#include "common/String.hpp"

#include <AccCtrl.h>
#include <atlcomcli.h>
#include <aclapi.h>
#include <iostream>
#include <libloaderapi.h>
#include <sddl.h>
#include <appmodel.h>
#include <shobjidl.h>
#include <processthreadsapi.h>

namespace fs = std::filesystem;

namespace sapphire::launcher {

    DebuggerApp::DebuggerApp(sapphire::sys::win::dword_t pid) : mGameProcessId(pid), mLauncherAppPath(MAX_PATH, {}) {
        HRESULT hResult = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        if (FAILED(hResult)) {
            ErrorBox(L"[launcher] CoInitializeEx 失败 (code: {})", hResult);
            throw std::runtime_error{"[launcher] CoInitializeEx 失败"};
        }

        GetModuleFileNameW(nullptr, mLauncherAppPath.data(), mLauncherAppPath.size());
        mLauncherAppDir = fs::path{mLauncherAppPath}.parent_path();
    }

    DebuggerApp::~DebuggerApp() noexcept {
        disableDebugging(L"Microsoft.MinecraftUWP_8wekyb3d8bbwe");
        if (mGameProcessHandle && mGameProcessHandle != INVALID_HANDLE_VALUE) {
            if (mInjectSuccess) {
                typedef LONG(NTAPI * NtResumeProcess_t)(HANDLE ProcessHandle);
                NtResumeProcess_t NtResumeProcess = (NtResumeProcess_t)GetProcAddress(GetModuleHandle(TEXT("ntdll")), "NtResumeProcess");
                NtResumeProcess(mGameProcessHandle);
            } else {
                TerminateProcess(mGameProcessHandle, 1);
            }
            CloseHandle(mGameProcessHandle);
        }

        CoUninitialize();
    }

    static void waitForDebuggerIfDebugMode() {
#ifdef SPHR_DEBUG
        if (IsDebuggerPresent()) return;

        MessageBoxW(nullptr, L"Waiting for debugger...\nClick 'OK' to continue.", L"Debug", MB_ICONINFORMATION);
        if (IsDebuggerPresent())
            __debugbreak();
#endif
    }

    int DebuggerApp::run() {
        waitForDebuggerIfDebugMode();
        auto dllPath = mLauncherAppDir / "bin" / "sapphire_bootloader.dll";
        if (!fs::exists(dllPath)) {
            ErrorBox(L"[Debugger] 未找到 sapphire_bootloader：{}", dllPath.c_str());
            return -1;
        }
        ensureDataDirExist();
        mGameProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, mGameProcessId);
        if (mGameProcessHandle == INVALID_HANDLE_VALUE) {
            ErrorBox(L"[Debugger] 无法获取游戏进程句柄 (pid: {}, code: {})", mGameProcessHandle, GetLastError());
            return -1;
        }

        auto ctx = sapphire::coro::IoContext::create(1);
        if (!ctx) {
            ErrorBox(L"[Debugger] 无法创建 IoContext. (code: {})", ctx.error().value());
            return -1;
        }

        syncWait(whenAll(
            pipeServerMain(*ctx),
            [](sapphire::coro::IoContext &ctx, sapphire::sys::win::handle_t hProcess, const std::filesystem::path &dllPath) -> sapphire::coro::Task<int> {
                if (!injectDll(hProcess, dllPath)) {
                    ctx.stop();
                    co_return -1;
                }
                co_return 0;
            }(*ctx, mGameProcessHandle, dllPath),
            [](sapphire::coro::IoContext &ctx) -> sapphire::coro::Task<int> {
                ctx.processEvents();
                co_return 0;
            }(*ctx)
        ));

        return 0;
    }

    sapphire::coro::Task<int> DebuggerApp::pipeServerMain(sapphire::coro::IoContext &ctx) {
        using namespace std::chrono_literals;
        sapphire::ScopeGuard guard{[&]() { ctx.stop(); }};

        std::cout << "[Debugger] Pipe connected!\n";

        sapphire::coro::AsyncScope scope;

        auto optPipe = ipc::backend::Pipe::create(L"\\\\.\\pipe\\SapphireSignalPipe.Bootloader", ctx);
        if (!optPipe) {
            ErrorBox(L"[Debugger] 无法创建通信管道 (\\\\.\\pipe\\SapphireSignalPipe.Bootloader), msg: {}", stringToWString(optPipe.error().message()));
            co_return -1;
        }
        auto optPipe2 = ipc::backend::Pipe::create(L"\\\\.\\pipe\\SapphireSignalPipe.Core", ctx);
        if (!optPipe2) {
            ErrorBox(L"[Debugger] 无法创建通信管道 (\\\\.\\pipe\\SapphireSignalPipe.Core), msg: {}", stringToWString(optPipe2.error().message()));
            co_return -1;
        }
        scope.spawn(handlePipeConnection(ctx, std::move(*optPipe)));
        scope.spawn(handlePipeConnection(ctx, std::move(*optPipe2)));

        co_await scope.join();

        if (mInjectSuccess) {
            std::cout << "[Debugger] Pipe disconnected!\n";
            std::cout << "[Debugger] All task done. Debugger will be closed 1s later.\n";
            std::this_thread::sleep_for(1s);
            co_return 0;
        } else {
            std::cerr << "[Debugger] Error occurred. Press any key to continue...\n";
            std::cin.ignore();
            co_return -1;
        }
    }

    sapphire::coro::Task<int> DebuggerApp::handlePipeConnection(
        sapphire::coro::IoContext &ctx,
        ipc::backend::Pipe         pipe
    ) {
        ipc::PipeChannel       channel{pipe};
        sapphire::ipc::Message msg;

        auto code = co_await pipe.listen();
        if (code) {
            std::cerr << "[Debugger] Pipe connection failed!\n";
            co_return -1;
        }

        while (co_await channel.recv(msg)) {
            auto status = msg.getStatus();
            if (status == sapphire::ipc::status::Handshake) {
                std::cout << "[Debugger] Pipe connection from: " << msg.getData() << '\n';
            } else if (status == sapphire::ipc::status::Handoff) {
                std::cout << "[Debugger] Pipe disconnecting from: " << msg.getData() << '\n';
                if (msg.getData() == "Sapphire Core") {
                    mInjectSuccess = true;
                }
                break;
            } else if (status == sapphire::ipc::status::Error) {
                std::cerr << "[Debugger] Error msg recieved: " << msg.getData() << '\n';
                co_return -1;
            } else if (status == sapphire::ipc::status::Success) {
                if (msg.getData().is_string())
                    std::cout << msg.getData().get<std::string>() << '\n';
                else
                    std::cout << "[Debugger] Invalid msg type: " << msg.getData().type_name() << '\n';
            }
        }
        co_return 0;
    }

    void DebuggerApp::ensureDataDirExist() {
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

    sapphire::sys::win::dword_t DebuggerApp::setPermissions(
        const std::wstring         &wstrFilePath,
        sapphire::sys::win::dword_t permission = GENERIC_READ | GENERIC_EXECUTE
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

    bool DebuggerApp::injectDll(
        sapphire::sys::win::handle_t hProcess,
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
            ErrorBox(L"[Debugger][{}]\n内存分配失败 (错误码: {})", dllPath.filename().c_str(), GetLastError());
            return false;
        }

        LPVOID pRemoteCode = pRemoteBuf.get();
        LPVOID pRemoteData = (char *)pRemoteBuf.get() + codeSize;

        if (!WriteProcessMemory(hProcess, pRemoteCode, gX64Stub, codeSize, nullptr)
            || !WriteProcessMemory(hProcess, pRemoteData, &params, dataSize, nullptr)) {
            ErrorBox(L"[Debugger][{}]\n写入内存失败 (错误码: {})", dllPath.filename().c_str(), GetLastError());
            return false;
        }

        AutoCloseHandle hThread{
            CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)pRemoteCode, pRemoteData, 0, nullptr),
        };
        if (!hThread) {
            ErrorBox(L"[Debugger][{}]\n创建远程线程失败 (错误码: {})", dllPath.filename().c_str(), GetLastError());
            return false;
        }
        WaitForSingleObject(hThread.get(), INFINITE);

        DWORD remoteErrorCode;
        if (!GetExitCodeThread(hThread.get(), &remoteErrorCode)) {
            ErrorBox(L"[Debugger][{}]\n无法获取远程线程返回值 (错误码: {})", dllPath.filename().c_str(), GetLastError());
            return false;
        }

        if (remoteErrorCode != ERROR_SUCCESS) {
            ErrorBox(L"[Debugger][{}]\nDll注入失败 (错误码: {})", dllPath.filename().c_str(), remoteErrorCode);
            return false;
        }

        return true;
    }

    void DebuggerApp::disableDebugging(const std::wstring &familyName) {
        std::wstring appFullName = LauncherApp::packageFullNameFromFamilyName(familyName);
        if (appFullName.empty())
            return;

        ATL::CComQIPtr<IPackageDebugSettings> debugSettings;

        HRESULT hResult = debugSettings.CoCreateInstance(CLSID_PackageDebugSettings, NULL, CLSCTX_ALL);
        if (hResult != S_OK) return;

        debugSettings->DisableDebugging(appFullName.c_str());
    }

} // namespace sapphire::launcher