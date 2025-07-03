#pragma once

#include <chrono>
#include <source_location>

namespace util {

    class TimerToken {
        friend class ScopedTimer;
        std::chrono::steady_clock::duration mDuration{};

    public:
        auto getDuration() const { return this->mDuration; }
    };

    class ScopedTimer {
    public:
        ScopedTimer(TimerToken &token) :
            mToken(token), mStartTime(std::chrono::steady_clock::now()) {}

        ~ScopedTimer() { mToken.mDuration += std::chrono::steady_clock::now() - mStartTime; }

    private:
        TimerToken                           &mToken;
        std::chrono::steady_clock::time_point mStartTime;
    };

} // namespace util