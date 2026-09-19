#pragma once

#include <chrono>

namespace sapphire {

    class TimerToken {
        using ClockType = std::chrono::steady_clock;

        friend class ScopedTimer;
        ClockType::duration mDuration{};

    public:
        constexpr ClockType::duration getDuration() const noexcept { return this->mDuration; }

        constexpr double getDurationNs() const noexcept {
            return std::chrono::duration<double, std::nano>(this->mDuration).count();
        }

        constexpr double getDurationMs() const noexcept {
            return std::chrono::duration<double, std::milli>(this->mDuration).count();
        }

        constexpr void reset() { mDuration = {}; }
    };

    class ScopedTimer {
        using ClockType = TimerToken::ClockType;

    public:
        ScopedTimer(TimerToken &token) noexcept :
            mToken(token), mStartTime(ClockType::now()) {}

        ~ScopedTimer() noexcept { mToken.mDuration += ClockType::now() - mStartTime; }

    private:
        TimerToken           &mToken;
        ClockType::time_point mStartTime;
    };

    class RecursiveTimerToken {
        using ClockType = std::chrono::steady_clock;

        friend class RecursiveScopedTimer;
        ClockType::duration mDuration{};
        size_t              mDepth = 0;

    public:
        constexpr ClockType::duration getDuration() const noexcept { return this->mDuration; }

        constexpr double getDurationNs() const noexcept {
            return std::chrono::duration<double, std::nano>(this->mDuration).count();
        }

        constexpr double getDurationMs() const noexcept {
            return std::chrono::duration<double, std::milli>(this->mDuration).count();
        }

        constexpr void reset() { mDuration = {}; }
    };

    class RecursiveScopedTimer {
        using ClockType = RecursiveTimerToken::ClockType;

    public:
        RecursiveScopedTimer(RecursiveTimerToken &token) noexcept :
            mToken(token), mStartTime(ClockType::now()) {
            if (mToken.mDepth++ == 0)
                mStartTime = ClockType::now();
        }

        ~RecursiveScopedTimer() noexcept {
            if (--mToken.mDepth == 0)
                mToken.mDuration += ClockType::now() - mStartTime;
        }

    private:
        RecursiveTimerToken  &mToken;
        ClockType::time_point mStartTime;
    };

} // namespace sapphire