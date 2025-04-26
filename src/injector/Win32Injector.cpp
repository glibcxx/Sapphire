
// #include "logger/LogBox.hpp"

// namespace fs = std::filesystem;
// using namespace LogBox;

// bool injectDll(DWORD pid, const fs::path &dllPath) {
//     std::wstring dllPathStr = dllPath;
//     HANDLE       hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
//     if (!hProcess) {
//         Error(L"打开进程失败 (错误码: {})", GetLastError());
//         return false;
//     }

//     // 在目标进程分配内存
//     LPVOID pRemotePath = VirtualAllocEx(
//         hProcess,
//         nullptr,
//         (dllPathStr.size() + 1) * sizeof(wchar_t),
//         MEM_COMMIT | MEM_RESERVE,
//         PAGE_READWRITE);
//     if (!pRemotePath) {
//         CloseHandle(hProcess);
//         Error(L"内存分配失败 (错误码: {})", GetLastError());
//         return false;
//     }

//     // 写入DLL路径
//     if (!WriteProcessMemory(
//             hProcess,
//             pRemotePath,
//             dllPathStr.c_str(),
//             (dllPathStr.size() + 1) * sizeof(wchar_t),
//             nullptr)) {
//         VirtualFreeEx(hProcess, pRemotePath, 0, MEM_RELEASE);
//         CloseHandle(hProcess);
//         Error(L"写入内存失败 (错误码: {})", GetLastError());
//         return false;
//     }

//     // 创建远程线程调用LoadLibraryW
//     HMODULE                hKernel32 = GetModuleHandleW(L"kernel32.dll");
//     LPTHREAD_START_ROUTINE pLoadLibrary = reinterpret_cast<LPTHREAD_START_ROUTINE>(GetProcAddress(hKernel32, "LoadLibraryW"));

//     HANDLE hThread = CreateRemoteThread(hProcess, nullptr, 0, pLoadLibrary, pRemotePath, 0, nullptr);
//     if (!hThread) {
//         VirtualFreeEx(hProcess, pRemotePath, 0, MEM_RELEASE);
//         CloseHandle(hProcess);
//         Error(L"创建远程线程失败 (错误码: {})", GetLastError());
//         return false;
//     }
//     // 等待线程结束
//     WaitForSingleObject(hThread, INFINITE);

//     // 清理资源
//     VirtualFreeEx(hProcess, pRemotePath, 0, MEM_RELEASE);
//     CloseHandle(hThread);
//     CloseHandle(hProcess);
//     return true;
// }

// int main() {
//     const fs::path currentDir = fs::current_path();

//     // 以挂起状态启动进程
//     STARTUPINFO         si = {sizeof(si)};
//     PROCESS_INFORMATION processInfo;
//     if (!CreateProcessW((currentDir / "HookMe.exe").c_str(), nullptr, nullptr, nullptr, FALSE,
//                         CREATE_SUSPENDED, nullptr, nullptr, &si, &processInfo)) {
//         Error(L"启动进程失败 (错误码: {})", GetLastError());
//         return 1;
//     }

//     // 注入 DLL
//     if (!injectDll(processInfo.dwProcessId, currentDir / "libHooker.dll")) {
//         TerminateProcess(processInfo.hProcess, 0);
//         CloseHandle(processInfo.hThread);
//         CloseHandle(processInfo.hProcess);
//         return 1;
//     }

//     // 恢复线程执行
//     ResumeThread(processInfo.hThread);

//     // 清理句柄
//     CloseHandle(processInfo.hThread);
//     CloseHandle(processInfo.hProcess);

//     Info(L"注入成功，目标进程已启动！");
//     return 0;
// }