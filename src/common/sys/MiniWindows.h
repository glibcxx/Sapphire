#pragma once

/*
    本文件提供了 windows.h 中的各种类型的等价实现，用来尽量避免在头文件中引入 windows.h
*/

#include <cstdint>

extern "C" {
struct HWND__;
struct HDC__;
struct HBRUSH__;
struct HINSTANCE__;

typedef struct _RTL_SRWLOCK          RTL_SRWLOCK;
typedef RTL_SRWLOCK                  SRWLOCK, *PSRWLOCK;
typedef struct _SECURITY_ATTRIBUTES *LPSECURITY_ATTRIBUTES;

void __stdcall                                   WakeByAddressSingle(void *Address);
void __stdcall                                   WakeByAddressAll(void *Address);
int __stdcall                                    WaitOnAddress(volatile void *Address, void *CompareAddress, unsigned long long AddressSize, unsigned long dwMilliseconds);
__declspec(dllimport) void __stdcall             AcquireSRWLockExclusive(PSRWLOCK SRWLock);
__declspec(dllimport) unsigned char __stdcall    TryAcquireSRWLockExclusive(PSRWLOCK SRWLock);
__declspec(dllimport) void __stdcall             ReleaseSRWLockExclusive(PSRWLOCK SRWLock);
__declspec(dllimport) void *__stdcall            GetCurrentThread();
__declspec(dllimport) unsigned __int64 __stdcall SetThreadAffinityMask(void *hThread, unsigned __int64 dwThreadAffinityMask);
__declspec(dllimport) long __stdcall             SetThreadDescription(void *hThread, const wchar_t *lpThreadDescription);
__declspec(dllimport) void *__stdcall            CreateEventW(LPSECURITY_ATTRIBUTES lpEventAttributes, int bManualReset, int bInitialState, const wchar_t *lpName);
__declspec(dllimport) int __stdcall              CloseHandle(void *hObject);
__declspec(dllimport) int __stdcall              SetEvent(void *hEvent);
__declspec(dllimport) unsigned long __stdcall    WaitForSingleObjectEx(void *hHandle, unsigned long dwMilliseconds, int bAlertable);
__declspec(dllimport) int __stdcall              SwitchToThread();
__declspec(dllimport) unsigned long __stdcall    SleepEx(unsigned long dwMilliseconds, int bAlertable);
__declspec(dllimport) unsigned long __stdcall    GetLastError(void);
}

namespace sapphire::sys::win {

    using handle_t = void *;
    using dword_t = unsigned long;
    using dword64_t = unsigned long long;
    using long_t = long;
    using ulong_t = unsigned long;
    using bool_t = int;
    using winbool_t = int;
    using byte_t = unsigned char;
    using word_t = unsigned short;
    using float_t = float;
    using longlong_t = __int64;
    using ulonglong_t = unsigned __int64;

    using uchar_t = unsigned char;

    using long_ptr_t = intptr_t;
    using ulong_ptr_t = uintptr_t;

    using wparam_t = uintptr_t;
    using lparam_t = intptr_t;
    using lresult_t = intptr_t;

    using hwnd_t = HWND__ *;
    using hmodule_t = HINSTANCE__ *;
    using hdc_t = HDC__ *;
    using hbrush_t = HBRUSH__ *;

    using lstatus_t = long_t;

    using lpcwstr_t = const wchar_t *;

} // namespace sapphire::sys::win
