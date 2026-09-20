#pragma once

#include "common/sys/MiniWindows.h"

namespace sapphire::coro::win {

    struct overlapped {
        sys::win::ulong_ptr_t Internal;
        sys::win::ulong_ptr_t InternalHigh;
        union {
            struct {
                sys::win::dword_t Offset;
                sys::win::dword_t OffsetHigh;
            };
            void *Pointer;
        };
        sys::win::handle_t hEvent;
    };
    using lpOverlapped = overlapped *;

    struct overlapped_entry {
        sys::win::ulong_ptr_t lpCompletionKey;
        lpOverlapped          lpOverlapped;
        sys::win::ulong_ptr_t Internal;
        sys::win::dword_t     dwNumberOfBytesTransferred;
    };
    using lpOverlapped_entry = overlapped_entry *;

    union large_integer {
        struct {
            sys::win::dword_t LowPart;
            sys::win::long_t  HighPart;
        };
        struct {
            sys::win::dword_t LowPart;
            sys::win::long_t  HighPart;
        } u;
        sys::win::longlong_t QuadPart;
    };
    using pLarge_integer = large_integer *;

    struct sockaddr {
        unsigned short sa_family;
        char           sa_data[14];
    };

    struct in_addr {
        union {
            struct {
                unsigned char s_b1, s_b2, s_b3, s_b4;
            } S_un_b;
            struct {
                unsigned short s_w1, s_w2;
            } S_un_w;
            unsigned long S_addr;
        } S_un;
    };
    using PIN_ADDR = in_addr *;
    using LPIN_ADDR = in_addr *;

    struct sockaddr_in {
        short          sin_family;
        unsigned short sin_port;
        in_addr        sin_addr;
        char           sin_zero[8];
    };

    struct in6_addr {
        union {
            unsigned char  Byte[16];
            unsigned short Word[8];
        } u;
    };

    struct scope_id {
        union {
            struct {
                unsigned long Zone  : 28;
                unsigned long Level : 4;
            } DUMMYSTRUCTNAME;
            unsigned long Value;
        } DUMMYUNIONNAME;
    };
    using pScope_id = scope_id *;

    struct sockaddr_in6_lh {
        unsigned short sin6_family;
        unsigned short sin6_port;
        unsigned long  sin6_flowinfo;
        in6_addr       sin6_addr;
        union {
            unsigned long sin6_scope_id;
            scope_id      sin6_scope_struct;
        };
    };

    using pSockaddr = sockaddr *;
    using lpSockaddr = sockaddr *;

    using pSockaddr_in = sockaddr_in *;
    using lpSockaddr_in = sockaddr_in *;

    using sockaddr_in6 = sockaddr_in6_lh;
    using pSockaddr_in6 = sockaddr_in6_lh *;
    using lpSockaddr_in6 = sockaddr_in6_lh *;

    struct sockaddr_storage_lh {
        unsigned short ss_family;
        char           __ss_pad1[6];
        __int64        __ss_align;
        char           __ss_pad2[112];
    };
    using pSockaddr_storage_lh = sockaddr_storage_lh *;
    using lpSockaddr_storage_lh = sockaddr_storage_lh *;

    using sockaddr_storage = sockaddr_storage_lh;

    struct WSAData {
        sys::win::word_t wVersion;
        sys::win::word_t wHighVersion;
        unsigned short   iMaxSockets;
        unsigned short   iMaxUdpDg;
        char            *lpVendorInfo;
        char             szDescription[256 + 1];
        char             szSystemStatus[128 + 1];
    };
    using lpWSAData = WSAData *;

    struct linger {
        unsigned short l_onoff;
        unsigned short l_linger;
    };

    struct WSABuf {
        unsigned long len;
        char         *buf;
    };
    using lpWSABuf = WSABuf *;

} // namespace sapphire::coro::win

extern "C" {

typedef struct _OVERLAPPED       OVERLAPPED, *LPOVERLAPPED;
typedef struct _OVERLAPPED_ENTRY OVERLAPPED_ENTRY, *LPOVERLAPPED_ENTRY;
typedef union _LARGE_INTEGER     LARGE_INTEGER, *PLARGE_INTEGER;
typedef struct WSAData           WSADATA, *LPWSADATA;

typedef struct _WSAPROTOCOL_INFOW WSAPROTOCOL_INFOW, *LPWSAPROTOCOL_INFOW;
typedef struct _WSABUF            WSABUF, *LPWSABUF;
typedef struct _OVERLAPPED       *LPWSAOVERLAPPED;
typedef void(__stdcall *LPWSAOVERLAPPED_COMPLETION_ROUTINE)(unsigned long dwError, unsigned long cbTransferred, LPWSAOVERLAPPED lpOverlapped, unsigned long dwFlags);

__declspec(dllimport) void *__stdcall CreateIoCompletionPort(void *FileHandle, void *ExistingCompletionPort, unsigned __int64 CompletionKey, unsigned long NumberOfConcurrentThreads);
__declspec(dllimport) int __stdcall   PostQueuedCompletionStatus(void *CompletionPort, unsigned long dwNumberOfBytesTransferred, unsigned __int64 dwCompletionKey, LPOVERLAPPED lpOverlapped);
__declspec(dllimport) int __stdcall   GetQueuedCompletionStatusEx(void *CompletionPort, LPOVERLAPPED_ENTRY lpCompletionPortEntries, unsigned long ulCount, unsigned long *ulNumEntriesRemoved, unsigned long dwMilliseconds, int fAlertable);
__declspec(dllimport) int __stdcall   CancelIoEx(void *hFile, LPOVERLAPPED lpOverlapped);

__declspec(dllimport) void *__stdcall CreateFileW(sapphire::sys::win::lpcwstr_t lpFileName, sapphire::sys::win::dword_t dwDesiredAccess, sapphire::sys::win::dword_t dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, sapphire::sys::win::dword_t dwCreationDisposition, sapphire::sys::win::dword_t dwFlagsAndAttributes, void *hTemplateFile);
__declspec(dllimport) int __stdcall   SetFileCompletionNotificationModes(void *FileHandle, unsigned char Flags);
__declspec(dllimport) int __stdcall   ReadFile(void *hFile, void *lpBuffer, unsigned long nNumberOfBytesToRead, unsigned long *lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
__declspec(dllimport) int __stdcall   WriteFile(void *hFile, const void *lpBuffer, unsigned long nNumberOfBytesToWrite, unsigned long *lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
__declspec(dllimport) int __stdcall   GetFileSizeEx(void *hFile, PLARGE_INTEGER lpFileSize);

__declspec(dllimport) int __stdcall closesocket(uintptr_t s);

__declspec(dllimport) unsigned short __stdcall htons(unsigned short hostshort);
__declspec(dllimport) unsigned short __stdcall ntohs(unsigned short netshort);

__declspec(dllimport) int __stdcall bind(unsigned __int64 s, const struct sockaddr *name, int namelen);
__declspec(dllimport) int __stdcall listen(unsigned __int64 s, int backlog);
__declspec(dllimport) int __stdcall getsockname(unsigned __int64 s, struct sockaddr *name, int *namelen);
__declspec(dllimport) int __stdcall shutdown(unsigned __int64 s, int how);

__declspec(dllimport) int __stdcall WSAGetLastError(void);
__declspec(dllimport) int __stdcall WSAStartup(unsigned short wVersionRequested, LPWSADATA lpWSAData);
__declspec(dllimport) int __stdcall WSACleanup(void);

__declspec(dllimport) unsigned __int64 __stdcall WSASocketW(int af, int type, int protocol, LPWSAPROTOCOL_INFOW lpProtocolInfo, unsigned int g, unsigned long dwFlags);
__declspec(dllimport) int __stdcall              setsockopt(unsigned __int64 s, int level, int optname, const char *optval, int optlen);

__declspec(dllimport) int __stdcall WSARecv(unsigned __int64 s, LPWSABUF lpBuffers, unsigned long dwBufferCount, unsigned long *lpNumberOfBytesRecvd, unsigned long *lpFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
__declspec(dllimport) int __stdcall WSARecvFrom(unsigned __int64 s, LPWSABUF lpBuffers, unsigned long dwBufferCount, unsigned long *lpNumberOfBytesRecvd, unsigned long *lpFlags, struct sockaddr *lpFrom, int *lpFromlen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
__declspec(dllimport) int __stdcall WSASend(unsigned __int64 s, LPWSABUF lpBuffers, unsigned long dwBufferCount, unsigned long *lpNumberOfBytesSent, unsigned long dwFlags, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
__declspec(dllimport) int __stdcall WSASendTo(unsigned __int64 s, LPWSABUF lpBuffers, unsigned long dwBufferCount, unsigned long *lpNumberOfBytesSent, unsigned long dwFlags, const struct sockaddr *lpTo, int iTolen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
}
