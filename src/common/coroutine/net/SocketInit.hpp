#pragma once

#include <atomic>
#include <system_error>
#include "../detail/WinDefs.h"
#include "common/sys/MiniWindows.h"

namespace sapphire::coro::net {

    class SocketInit {
        struct InitData {
            std::atomic_int initCount;
            std::atomic_int initResult;
        };

        SocketInit() {
            if (sInitData.initCount.fetch_add(1, std::memory_order_acquire) == 0) {
                coro::win::WSAData     winsockData;
                const sys::win::word_t requestedVersion = 514; // MAKEWORD(2, 2)
                const int              result = WSAStartup(requestedVersion, (LPWSADATA)&winsockData);
                if (result == -1) {
                    sInitData.initResult.store(result, std::memory_order_release);
                    throw std::system_error{WSAGetLastError(), std::system_category(), "WSAStartup failed"};
                }
            }
        }

        ~SocketInit() {
            if (sInitData.initCount.fetch_sub(1, std::memory_order_acq_rel) == 1) WSACleanup();
        }

        inline static InitData sInitData{};
        static SocketInit      sInit;
    };

    inline SocketInit SocketInit::sInit{}; // TODO: 跨 dll

} // namespace sapphire::net
