#pragma once

#include <chrono>
#include <source_location>

namespace util {

    class TimerToken {
        friend class ScopedTimer;
        std::chrono::steady_clock::duration mDuration{};

    public:
        constexpr auto getDuration() const noexcept { return this->mDuration; }

        constexpr double getDurationNs() const noexcept {
            return std::chrono::duration<double, std::nano>(this->mDuration).count();
        }

        constexpr double getDurationMs() const noexcept {
            return std::chrono::duration<double, std::milli>(this->mDuration).count();
        }
    };

    class ScopedTimer {
    public:
        ScopedTimer(TimerToken &token) noexcept :
            mToken(token), mStartTime(std::chrono::steady_clock::now()) {}

        ~ScopedTimer() noexcept { mToken.mDuration += std::chrono::steady_clock::now() - mStartTime; }

    private:
        TimerToken                           &mToken;
        std::chrono::steady_clock::time_point mStartTime;
    };

} // namespace util