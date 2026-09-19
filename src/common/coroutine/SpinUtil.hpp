#pragma once

#include <cstdint>
#include "common/sys/MiniWindows.h"

#pragma push_macro("CPU_PAUSE")

#undef CPU_PAUSE
#if defined(_WIN32_WINNT) || defined(_WIN32)
#    include <intrin.h>
#    define CPU_PAUSE() _mm_pause()
#elif defined(__GNUC__) || defined(__clang__)
#    define CPU_PAUSE() __builtin_ia32_pause()
#else
#    define CPU_PAUSE() std::this_thread::yield()
#endif

namespace sapphire::coro::detail {

    class SpinUtil {
    public:
        static void spinOnce(std::uint32_t spunCount, std::uint32_t MAX_SPIN_COUNT = 16) noexcept {
            if (spunCount < MAX_SPIN_COUNT) [[likely]] {
                const std::uint32_t loopCount = 2u << spunCount;
                for (std::uint32_t i = 0; i < loopCount; ++i) {
                    CPU_PAUSE();
                    CPU_PAUSE();
                }
            } else {
                yieldThread(spunCount, MAX_SPIN_COUNT);
            }
        }

        static void yieldThread(std::uint32_t spunCount, std::uint32_t MAX_SPIN_COUNT = 16) noexcept {
            const auto yieldCount = spunCount - MAX_SPIN_COUNT;
            if (yieldCount % 20 == 19) {
                SleepEx(1, 0);
            } else if (yieldCount % 5 == 4) {
                SleepEx(0, 0);
            } else {
                SwitchToThread();
            }
        }
    };

} // namespace sapphire::coro::detail

#pragma pop_macro("CPU_PAUSE") // NOLINT
