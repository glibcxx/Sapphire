#pragma once

#include "StackTrace.hpp"
#include "macros/Macros.h"
#include <csignal>
#include <string_view>
#include <winbase.h>
#include <windows.h>
#include <winnt.h>
#include <dbghelp.h>
#include <strsafe.h>

#pragma comment(lib, "kernel32.lib")

extern "C" {
void **__current_exception();
void **__current_exception_context();
}

namespace sapphire::debug {

    class StackProber {
    public:
        static constexpr size_t MIN_SAFE_STACK_MARGIN = 2048;

        [[nodiscard]] SPHR_FORCE_INLINE static bool isSufficient(DWORD exceptionCode, size_t requiredMargin = MIN_SAFE_STACK_MARGIN) noexcept {
            if (exceptionCode != EXCEPTION_STACK_OVERFLOW) {
                return true;
            }

            const auto *tib = reinterpret_cast<const NT_TIB *>(NtCurrentTeb());
            if (!tib) {
                return false;
            }

            const uintptr_t currentSp = reinterpret_cast<uintptr_t>(_AddressOfReturnAddress());
            const uintptr_t stackLimit = reinterpret_cast<uintptr_t>(tib->StackLimit);

            return (currentSp > stackLimit) && ((currentSp - stackLimit) >= requiredMargin);
        }

        struct Details {
            uintptr_t mCurrentSp = 0;
            uintptr_t mStackLimit = 0;
            uintptr_t mStackBase = 0;
            size_t    mMarginBytes = 0;
        };

        [[nodiscard]] static Details probeDetails() noexcept {
            Details     d{};
            const auto *tib = reinterpret_cast<const NT_TIB *>(NtCurrentTeb());
            if (!tib) {
                return d;
            }

            d.mCurrentSp = reinterpret_cast<uintptr_t>(_AddressOfReturnAddress());
            d.mStackLimit = reinterpret_cast<uintptr_t>(tib->StackLimit);
            d.mStackBase = reinterpret_cast<uintptr_t>(tib->StackBase);
            d.mMarginBytes = (d.mCurrentSp > d.mStackLimit) ? (d.mCurrentSp - d.mStackLimit) : 0;
            return d;
        }
    };

    class LowStackThreadLauncher {
    public:
        using PfnNtCreateThreadEx = LONG(NTAPI *)(PHANDLE ThreadHandle, ACCESS_MASK DesiredAccess, PVOID ObjectAttributes, HANDLE ProcessHandle, PVOID StartRoutine, PVOID Argument, ULONG CreateFlags, SIZE_T ZeroBits, SIZE_T StackSize, SIZE_T MaximumStackSize, PVOID AttributeList);

        [[nodiscard]] static PfnNtCreateThreadEx resolveNtCreateThreadEx() noexcept {
            HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
            if (!hNtdll) {
                return nullptr;
            }
            return reinterpret_cast<PfnNtCreateThreadEx>(
                GetProcAddress(hNtdll, "NtCreateThreadEx")
            );
        }

        SPHR_FORCE_INLINE static HANDLE launch(
            PfnNtCreateThreadEx    pfnNtCreateThreadEx,
            LPTHREAD_START_ROUTINE pfnRoutine,
            void                  *pContext,
            size_t                 stackReserveSize = 0
        ) noexcept {
            HANDLE hThread = nullptr;
            if (pfnNtCreateThreadEx) {
                const LONG status = pfnNtCreateThreadEx(
                    &hThread,
                    THREAD_ALL_ACCESS,
                    nullptr,
                    reinterpret_cast<HANDLE>(static_cast<intptr_t>(-1)),
                    reinterpret_cast<PVOID>(pfnRoutine),
                    pContext,
                    0,
                    0,
                    0,
                    stackReserveSize,
                    nullptr
                );
            }
            return hThread;
        }

    private:
        LowStackThreadLauncher() {
            HMODULE hNtdll = GetModuleHandleW(L"ntdll.dll");
            if (hNtdll) {
                mPfnNtCreateThreadEx = reinterpret_cast<PfnNtCreateThreadEx>(
                    GetProcAddress(hNtdll, "NtCreateThreadEx")
                );
            }
        }

        ~LowStackThreadLauncher() = default;

    private:
        PfnNtCreateThreadEx mPfnNtCreateThreadEx = nullptr;
    };

    struct CrashContext {
        EXCEPTION_POINTERS *mExceptionPointers = nullptr;
        DWORD               mCrashingThreadId = 0;
        DWORD               mExceptionCode = 0;
    };

    class CrashHandler {
    public:
        using CallbackType = bool (*)(std::string_view);

        CrashHandler(CallbackType callback = nullptr) {
            Symbolizer::instance();

            _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);

            std::set_terminate(globalTerminateHandler);
            _set_purecall_handler(globalPureCallHandler);
            _set_invalid_parameter_handler(globalInvalidParameterHandler);

            signal(SIGABRT, globalSignalHandler);
            signal(SIGINT, globalSignalHandler);
            signal(SIGTERM, globalSignalHandler);
            signal(SIGFPE, globalSignalHandler);
            signal(SIGILL, globalSignalHandler);
            signal(SIGSEGV, globalSignalHandler);

            sPreviousFilter = SetUnhandledExceptionFilter(globalUnhandledExceptionFilter);
            sPfnNtCreateThreadEx = LowStackThreadLauncher::resolveNtCreateThreadEx();
            sCallback = callback;
        }

    private:
        // 此函数在开启优化后栈大小为 104 字节，为理论最优结果，尽可能把栈留给 NtCreateThreadEx
        static LONG __stdcall globalUnhandledExceptionFilter(EXCEPTION_POINTERS *ep) noexcept {
            if (_InterlockedCompareExchange(&sHandlingCrash, 1, 0) != 0) {
                Sleep(INFINITE);
                return EXCEPTION_EXECUTE_HANDLER;
            }

            sContext.mExceptionPointers = ep;
            sContext.mCrashingThreadId = GetCurrentThreadId();

            DWORD exceptionCode = 0;
            if (ep && ep->ExceptionRecord) {
                exceptionCode = ep->ExceptionRecord->ExceptionCode;
            }
            sContext.mExceptionCode = exceptionCode;

            // 探测栈可用容量
            const bool sufficient = StackProber::isSufficient(exceptionCode);

            if (sufficient) {
                sWorkerThreadHandle = LowStackThreadLauncher::launch(
                    sPfnNtCreateThreadEx,
                    workerThreadEntry,
                    &sContext
                );

                if (sWorkerThreadHandle) {
                    WaitForSingleObject(sWorkerThreadHandle, INFINITE);
                    CloseHandle(sWorkerThreadHandle);
                    sWorkerThreadHandle = nullptr;
                    return EXCEPTION_EXECUTE_HANDLER;
                }
            }

            fallbackMinimalHandler();
        }

        [[noreturn]] SPHR_NOINLINE static void fallbackMinimalHandler() noexcept {
            constexpr char msg[] = "\r\n[FATAL] Stack Overflow: physical stack depleted. Terminating.\r\n";
            HANDLE         hStdErr = GetStdHandle(STD_ERROR_HANDLE);
            if (hStdErr && hStdErr != INVALID_HANDLE_VALUE) {
                DWORD written = 0;
                WriteFile(hStdErr, msg, sizeof(msg) - 1, &written, nullptr);
            }
            TerminateProcess(GetCurrentProcess(), EXCEPTION_STACK_OVERFLOW);
        } // NOLINT

        static void globalTerminateHandler() noexcept {
            auto *pRecord = static_cast<EXCEPTION_RECORD *>(*__current_exception());
            auto *pContext = static_cast<CONTEXT *>(*__current_exception_context());
            if (pRecord && pContext) {
                // 未处理异常/noexcept违规 导致的terminate
                EXCEPTION_POINTERS ep{};
                ep.ExceptionRecord = pRecord;
                ep.ContextRecord = pContext;

                globalUnhandledExceptionFilter(&ep);
                return;
            }
            // 一般的terminate
            CONTEXT ctx{};
            RtlCaptureContext(&ctx);
            EXCEPTION_RECORD rec{};
            rec.ExceptionCode = 0xE0000001;
            rec.ExceptionAddress = reinterpret_cast<void *>(ctx.Rip);

            EXCEPTION_POINTERS ep{};
            ep.ContextRecord = &ctx;
            ep.ExceptionRecord = &rec;
            globalUnhandledExceptionFilter(&ep);
        }

        static void globalPureCallHandler() noexcept {
            CONTEXT ctx{};
            RtlCaptureContext(&ctx);
            EXCEPTION_RECORD rec{};
            rec.ExceptionCode = 0xE0000002;
            rec.ExceptionAddress = reinterpret_cast<void *>(ctx.Rip);

            EXCEPTION_POINTERS ep{};
            ep.ContextRecord = &ctx;
            ep.ExceptionRecord = &rec;
            globalUnhandledExceptionFilter(&ep);
        }

        static void globalInvalidParameterHandler(const wchar_t *expression, const wchar_t *function, const wchar_t *file, unsigned int line, uintptr_t reserved) noexcept {
            CONTEXT ctx{};
            RtlCaptureContext(&ctx);
            EXCEPTION_RECORD rec{};
            rec.ExceptionCode = 0xE0000003;
            rec.ExceptionAddress = reinterpret_cast<void *>(ctx.Rip);

            EXCEPTION_POINTERS ep{};
            ep.ContextRecord = &ctx;
            ep.ExceptionRecord = &rec;
            globalUnhandledExceptionFilter(&ep);
        }

        static void globalSignalHandler(int signal) noexcept {
            EXCEPTION_POINTERS *ep = static_cast<EXCEPTION_POINTERS *>(_pxcptinfoptrs);
            if (ep) {
                globalUnhandledExceptionFilter(ep);
                return;
            }
            auto *pRecord = static_cast<EXCEPTION_RECORD *>(*__current_exception());
            auto *pContext = static_cast<CONTEXT *>(*__current_exception_context());
            if (pRecord && pContext) {
                // 未处理异常/noexcept违规 导致的terminate
                EXCEPTION_POINTERS ep{};
                ep.ExceptionRecord = pRecord;
                ep.ContextRecord = pContext;

                globalUnhandledExceptionFilter(&ep);
                return;
            }
            CONTEXT ctx{};
            RtlCaptureContext(&ctx);
            EXCEPTION_RECORD rec{};
            rec.ExceptionCode = 0xE0000004;
            rec.ExceptionAddress = reinterpret_cast<void *>(ctx.Rip);

            EXCEPTION_POINTERS syntheticEp{};
            syntheticEp.ContextRecord = &ctx;
            syntheticEp.ExceptionRecord = &rec;
            globalUnhandledExceptionFilter(&syntheticEp);
        }

        [[nodiscard]] static constexpr std::string_view getExceptionDescription(DWORD code) noexcept {
            switch (code) {
            case EXCEPTION_ACCESS_VIOLATION: return "EXCEPTION_ACCESS_VIOLATION";
            case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
            case EXCEPTION_BREAKPOINT: return "EXCEPTION_BREAKPOINT";
            case EXCEPTION_DATATYPE_MISALIGNMENT: return "EXCEPTION_DATATYPE_MISALIGNMENT";
            case EXCEPTION_FLT_DIVIDE_BY_ZERO: return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
            case EXCEPTION_ILLEGAL_INSTRUCTION: return "EXCEPTION_ILLEGAL_INSTRUCTION";
            case EXCEPTION_IN_PAGE_ERROR: return "EXCEPTION_IN_PAGE_ERROR";
            case EXCEPTION_INT_DIVIDE_BY_ZERO: return "EXCEPTION_INT_DIVIDE_BY_ZERO";
            case EXCEPTION_INT_OVERFLOW: return "EXCEPTION_INT_OVERFLOW";
            case EXCEPTION_INVALID_DISPOSITION: return "EXCEPTION_INVALID_DISPOSITION";
            case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
            case EXCEPTION_PRIV_INSTRUCTION: return "EXCEPTION_PRIV_INSTRUCTION";
            case EXCEPTION_SINGLE_STEP: return "EXCEPTION_SINGLE_STEP";
            case EXCEPTION_STACK_OVERFLOW: return "EXCEPTION_STACK_OVERFLOW";
            case 0xE06D7363: return "MSVC_CPP_EXCEPTION";
            case 0xE0000001: return "CRT_TERMINATE_CALLED";
            case 0xE0000002: return "CRT_PURECALL";
            case 0xE0000003: return "CRT_INVALID_PARAMETER";
            default: return "UNKNOWN_EXCEPTION";
            }
        }

        template <std::output_iterator<const char &> OutIter>
        static OutIter formatRegisters(OutIter out, const CONTEXT &ctx) {
            // clang-format off
#if defined(_M_X64) || defined(__x86_64__)
            std::format_to(out,
                "  RAX: 0x{:016X}  RBX: 0x{:016X}  RCX: 0x{:016X}  RDX: 0x{:016X}\n"
                "  RSI: 0x{:016X}  RDI: 0x{:016X}  RBP: 0x{:016X}  RSP: 0x{:016X}\n"
                "  R8 : 0x{:016X}  R9 : 0x{:016X}  R10: 0x{:016X}  R11: 0x{:016X}\n"
                "  R12: 0x{:016X}  R13: 0x{:016X}  R14: 0x{:016X}  R15: 0x{:016X}\n"
                "  RIP: 0x{:016X}  EFLAGS: 0x{:08X}\n",
                ctx.Rax, ctx.Rbx, ctx.Rcx, ctx.Rdx,
                ctx.Rsi, ctx.Rdi, ctx.Rbp, ctx.Rsp,
                ctx.R8,  ctx.R9,  ctx.R10, ctx.R11,
                ctx.R12, ctx.R13, ctx.R14, ctx.R15,
                ctx.Rip, ctx.EFlags
            );
#elif defined(_M_IX86) || defined(__i386__)
            std::format_to(out,
                "  EAX: 0x{:08X}  EBX: 0x{:08X}  ECX: 0x{:08X}  EDX: 0x{:08X}\n"
                "  ESI: 0x{:08X}  EDI: 0x{:08X}  EBP: 0x{:08X}  ESP: 0x{:08X}\n"
                "  EIP: 0x{:08X}  EFLAGS: 0x{:08X}\n",
                ctx.Eax, ctx.Ebx, ctx.Ecx, ctx.Edx,
                ctx.Esi, ctx.Edi, ctx.Ebp, ctx.Esp,
                ctx.Eip, ctx.EFlags
            );
#endif
            // clang-format on
            return out;
        }

        template <std::output_iterator<const char &> OutIter>
        static OutIter formatExceptionDetails(OutIter out, const EXCEPTION_RECORD &rec) {
            const auto desc = getExceptionDescription(rec.ExceptionCode);
            std::format_to(out, "  Code       : 0x{:08X} ({})\n", rec.ExceptionCode, desc);
            std::format_to(out, "  Fault Addr : {:p}\n", rec.ExceptionAddress);

            if (rec.ExceptionCode == EXCEPTION_ACCESS_VIOLATION && rec.NumberParameters >= 2) {
                const auto       accessType = rec.ExceptionInformation[0];
                const auto       targetAddr = rec.ExceptionInformation[1];
                std::string_view typeStr = (accessType == 0) ? "Reading" : (accessType == 1) ? "Writing"
                                                                     : (accessType == 8)     ? "Executing (DEP)"
                                                                                             : "Unknown Access to";
                std::format_to(out, "  Violation  : {} address 0x{:016X}\n", typeStr, targetAddr);
            }
            return out;
        }

        static void writeToConsoleAndDebug(std::string_view message) noexcept {
            HANDLE hStdErr = GetStdHandle(STD_ERROR_HANDLE);
            if (hStdErr && hStdErr != INVALID_HANDLE_VALUE) {
                DWORD written = 0;
                WriteFile(hStdErr, message.data(), static_cast<DWORD>(message.size()), &written, nullptr);
            }
            OutputDebugStringA(message.data());
        }

        static DWORD WINAPI workerThreadEntry(LPVOID lpParam) noexcept {
            auto *context = reinterpret_cast<CrashContext *>(lpParam);
            if (!context) {
                TerminateProcess(GetCurrentProcess(), static_cast<UINT>(-1));
                return 0;
            }

            std::string report;
            auto        it = std::back_inserter(report);

            std::format_to(it, "=======================================================\n");
            std::format_to(it, "                 FATAL CRASH DETECTED                 \n");
            std::format_to(it, "=======================================================\n");
            std::format_to(it, "[Process Info]\n");
            std::format_to(it, "  Process ID : {}\n", GetCurrentProcessId());
            std::format_to(it, "  Crashing TID: {}\n", context->mCrashingThreadId);
            std::format_to(it, "  Imagebase: {}\n", (void *)GetModuleHandleW(nullptr));

            const auto *ep = context->mExceptionPointers;
            if (ep && ep->ExceptionRecord) {
                std::format_to(it, "\n[Exception Summary]\n");
                formatExceptionDetails(it, *ep->ExceptionRecord);
            }

            if (ep && ep->ContextRecord) {
                std::format_to(it, "\n[CPU Registers Snapshot]\n");
                formatRegisters(it, *ep->ContextRecord);

                HANDLE hCrashingThread = OpenThread(
                    THREAD_QUERY_INFORMATION | THREAD_GET_CONTEXT,
                    FALSE,
                    context->mCrashingThreadId
                );

                std::format_to(it, "\n[Call Stack Trace]\n");
                auto trace = StackTrace::fromContext(
                    *ep->ContextRecord,
                    hCrashingThread ? hCrashingThread : GetCurrentThread()
                );

                if (trace.empty()) {
                    std::format_to(it, "  <No stack frames captured>\n");
                } else {
                    trace.toString(it, 2);
                }

                if (hCrashingThread) {
                    CloseHandle(hCrashingThread);
                }
            }

            std::format_to(it, "=======================================================\n\n");

            if (!sCallback || !sCallback(report))
                writeToConsoleAndDebug(report);
            TerminateProcess(GetCurrentProcess(), context->mExceptionCode);
            return 0;
        }

        inline static LowStackThreadLauncher::PfnNtCreateThreadEx sPfnNtCreateThreadEx = nullptr;
        inline static LPTOP_LEVEL_EXCEPTION_FILTER                sPreviousFilter = nullptr;
        inline static volatile long                               sHandlingCrash = 0;
        inline static HANDLE                                      sWorkerThreadHandle = nullptr;
        inline static CrashContext                                sContext{};

        inline static CallbackType sCallback;
    };

} // namespace sapphire::debug