#include "CrashLog.h"
#include "SDK/api/sapphire/logger/Logger.h"
#include "SDK/api/sapphire/platform/Environment.h"
#include "SDK/core/Runtime.h"

#pragma comment(lib, "dbghelp.lib")

namespace fs = std::filesystem;

[[noreturn]] LONG WINAPI sapphire::core::CrashLogger::sehHandler(EXCEPTION_POINTERS *ep) {
    sMessageBuffer.write("Exception Code: {:#X}", ep->ExceptionRecord->ExceptionCode);
    handleCrash(ep, sMessageBuffer.view());
}

[[noreturn]] void sapphire::core::CrashLogger::signalHandler(int sig) {
    sMessageBuffer.write("Signal Received (Code: {:#X})", sig);
    handleCrash(nullptr, sMessageBuffer.view());
}

[[noreturn]] void sapphire::core::CrashLogger::pureCallHandler() {
    handleCrash(nullptr, "Pure Virtual Function Call");
}

[[noreturn]] void sapphire::core::CrashLogger::invalidParamHandler(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t) {
    handleCrash(nullptr, "CRT Invalid Parameter");
}

[[noreturn]] void sapphire::core::CrashLogger::terminateHandler() {
    // 尝试获取 C++ 异常信息
    const char        *msg = "Unknown";
    std::exception_ptr ptr = std::current_exception();
    if (ptr) {
        try {
            std::rethrow_exception(ptr);
        } catch (const std::exception &e) {
            e,
                sLogBuffer.write("Unhandled Exception, e.what(): '{}'\n", e.what());
        } catch (...) {
            sLogBuffer.write("Unhandled Unknown Exception\n");
        }
    }
    handleCrash(nullptr, "std::terminate (Uncaught C++ Exception)");
}

[[noreturn]] void sapphire::core::CrashLogger::handleCrash(EXCEPTION_POINTERS *ep, std::string_view reason) {
    bool expected = false;
    if (!sIsCrashing.compare_exchange_strong(expected, true)) {
        TerminateProcess(GetCurrentProcess(), 0);
    }

    sLogBuffer.write("====================================================\n"
                     " [Sapphire Crash Log] CRASH DETECTED\n"
                     "====================================================\n"
                     ">>>>>>\n"
                     "Time:      {:%Y.%m.%d-%H:%M:%S}\n"
                     "Reason:    {}\n"
                     "Thread:    {}\n"
                     "Imagebase: {:#X}\n\n",
                     std::chrono::system_clock::now(),
                     reason,
                     GetCurrentThreadId(),
                     sImagebase);

    if (ep) {
        walkStack(ep->ContextRecord);
    } else {
        CONTEXT ctx = {0};
        RtlCaptureContext(&ctx);
        walkStack(&ctx);
    }
    sLogBuffer.write("<<<<<<\n");

    sapphire::core::Runtime::getInstance().getCrashLogger().flushToFile();

    TerminateProcess(GetCurrentProcess(), 1);
} // NOLINT: Function declared 'noreturn' should not return

sapphire::core::CrashLogger::CrashLogger() {
    std::chrono::zoned_time zt{std::chrono::current_zone(), std::chrono::system_clock::now()};
    auto                   &env = sapphire::platform::Environment::getInstance();
    sImagebase = env.getImagebase();
    auto                 &home = env.getSapphireHomePath();
    std::filesystem::path logDir = home / "crash";
    std::filesystem::create_directories(logDir);
    mLogPath = logDir / std::format(L"crash_dump_{:%Y.%m.%d-%H_%M_%S}.log", zt);
    sUserPdbSearchPaths = std::format("{};{}", home.parent_path().string(), (home / "mods").string());

    SetUnhandledExceptionFilter(sehHandler);
    _set_purecall_handler(pureCallHandler);
    _set_invalid_parameter_handler(invalidParamHandler);
    signal(SIGABRT, signalHandler);
    signal(SIGFPE, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGSEGV, signalHandler);
    std::set_terminate(terminateHandler);
}

void sapphire::core::CrashLogger::walkStack(CONTEXT *ctx) {
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();

    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME | SYMOPT_FAIL_CRITICAL_ERRORS | SYMOPT_DEFERRED_LOADS);
    SymInitialize(process, sUserPdbSearchPaths.c_str(), TRUE);

    STACKFRAME64 frame = {};
    frame.AddrPC.Offset = ctx->Rip;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = ctx->Rbp;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Offset = ctx->Rsp;
    frame.AddrStack.Mode = AddrModeFlat;

    // todo: dump register context

    sLogBuffer.write("Call Stack:\n");

    CONTEXT _ctx = *ctx;

    for (int i = 0; i < 64; ++i) {
        if (!StackWalk64(
                IMAGE_FILE_MACHINE_AMD64,
                process,
                thread,
                &frame,
                &_ctx,
                nullptr,
                SymFunctionTableAccess64,
                SymGetModuleBase64,
                nullptr
            )) {
            break;
        }

        if (frame.AddrPC.Offset == 0) break;

        alignas(SYMBOL_INFO) std::byte symBuf[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
        auto                          *symbol = reinterpret_cast<SYMBOL_INFO *>(symBuf);
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = MAX_SYM_NAME;

        DWORD64          displacement = 0;
        std::string_view funcName = "<Unknown>";
        if (SymFromAddr(process, frame.AddrPC.Offset, &displacement, symbol)) {
            funcName = symbol->Name;
        }

        IMAGEHLP_LINE64 line = {0};
        line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
        DWORD lineDisp = 0;

        if (displacement)
            sLogBuffer.write("  [{}]{:#X}: {}+{:#X}", i, frame.AddrPC.Offset, funcName, displacement);
        else
            sLogBuffer.write("  [{}]{:#X}: {}", i, frame.AddrPC.Offset, funcName);

        if (SymGetLineFromAddr64(process, frame.AddrPC.Offset, &lineDisp, &line)) {
            sLogBuffer.write(" ({}:{})", line.FileName, line.LineNumber);
        } else {
            HMODULE hMod = nullptr;
            if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)frame.AddrPC.Offset, &hMod)) {
                char modName[MAX_PATH];
                GetModuleFileNameA(hMod, modName, MAX_PATH);
                fs::path p(modName);
                sLogBuffer.write(" ({}+{:#X})", p.filename().string(), frame.AddrPC.Offset - (DWORD64)hMod);
            }
        }
        sLogBuffer.write("\n");
    }

    SymCleanup(process);
}

void sapphire::core::CrashLogger::flushToFile() {
    HANDLE hFile = CreateFileW(
        mLogPath.c_str(),
        GENERIC_WRITE,
        FILE_SHARE_READ,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD written;
        WriteFile(hFile, sLogBuffer.c_str(), static_cast<DWORD>(sLogBuffer.size()), &written, nullptr);
        CloseHandle(hFile);
    }
}