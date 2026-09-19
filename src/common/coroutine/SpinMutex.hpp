#pragma once

#include <atomic>
#include <cstdint>
#include <thread>
#include "SpinUtil.hpp"

namespace sapphire::coro {

    class SpinMutex {
        static constexpr std::uint32_t MAX_SPIN_COUNT = 16;

    public:
        SpinMutex() noexcept = default;

        bool try_lock() noexcept { return !mLocked.exchange(true, std::memory_order_acquire); }

        void lock() noexcept {
            while (!try_lock()) {
                static const std::uint32_t INITAL_SPIN_COUNT =
                    std::thread::hardware_concurrency() > 1 ? 0 : MAX_SPIN_COUNT;

                std::uint32_t spunCount = INITAL_SPIN_COUNT;

                while (mLocked.load(std::memory_order_relaxed)) {
                    detail::SpinUtil::spinOnce(spunCount, MAX_SPIN_COUNT);
                    ++spunCount;
                    if (spunCount == 0) spunCount = MAX_SPIN_COUNT;
                }
            }
        }

        void unlock() noexcept { mLocked.store(false, std::memory_order_release); }

    private:
        std::atomic<bool> mLocked;
    };

} // namespace sapphire::coro