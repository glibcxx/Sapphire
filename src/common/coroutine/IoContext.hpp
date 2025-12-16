#pragma once

#include <Windows.h>
#include <coroutine>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>

namespace sapphire::coro {

    namespace detail {

        struct AutoHandle {
            HANDLE h = nullptr;
            AutoHandle(HANDLE h) : h(h) {}
            ~AutoHandle() {
                if (h) CloseHandle(h);
            }
            operator HANDLE() const { return h; }
        };

    } // namespace detail

    // aka. EventLoop/IoService
    class IoContext {
        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = Clock::time_point;

        struct TimerEntry {
            TimePoint               deadline;
            std::coroutine_handle<> handle;

            bool operator>(const TimerEntry &other) const {
                return deadline > other.deadline;
            }
        };

    public:
        IoContext() :
            mWakeEvent(CreateEvent(NULL, FALSE, FALSE, NULL)),
            mTimerHandle(CreateWaitableTimerEx(NULL, NULL, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS)) {}

        ~IoContext() noexcept = default;

        void processEvents() {
            HANDLE handles[2] = {mWakeEvent, mTimerHandle};

            while (mRunning.load(std::memory_order_acquire)) {
                bool haveTimer = false;

                {
                    std::lock_guard lock(mMutex);
                    if (!mTimers.empty()) {
                        auto now = Clock::now();
                        if (now < mTimers.top().deadline) {
                            auto diff = mTimers.top().deadline - now;
                            // 转换为 100ns 单位 (Windows 内核时间单位)
                            int64_t interval = std::chrono::duration_cast<std::chrono::nanoseconds>(diff).count() / 100;

                            // 负数表示相对时间
                            LARGE_INTEGER li;
                            li.QuadPart = -interval;

                            if (SetWaitableTimer(mTimerHandle, &li, 0, NULL, NULL, FALSE)) {
                                haveTimer = true;
                            }
                        }
                    }
                }

                DWORD waitCount = haveTimer ? 2 : 1;

                DWORD result = WaitForMultipleObjects(waitCount, handles, FALSE, INFINITE);

                std::vector<std::coroutine_handle<>> readyTasks;
                {
                    std::lock_guard lock(mMutex);
                    TimePoint       now = Clock::now();
                    while (!mTimers.empty() && mTimers.top().deadline <= now) {
                        readyTasks.push_back(mTimers.top().handle);
                        mTimers.pop();
                    }
                }

                for (auto &h : readyTasks) h.resume();
            }
        }

        void stop() noexcept {
            if (!mRunning.exchange(false)) return;
            SetEvent(mWakeEvent);
        }

        template <typename Rep, typename Period>
        [[nodiscard]] auto scheduleAfter(const std::chrono::duration<Rep, Period> &dura) noexcept {
            struct TimerAwaiter {
                IoContext &io;
                TimePoint  deadline;

                constexpr bool await_ready() const noexcept { return false; }
                void           await_suspend(std::coroutine_handle<> h) { io.addTimer(deadline, h); }
                constexpr void await_resume() const noexcept {}
            };
            return TimerAwaiter{*this, Clock::now() + dura};
        }

    private:
        void addTimer(TimePoint deadline, std::coroutine_handle<> h) {
            bool needWake = false;
            {
                std::lock_guard lock(mMutex);
                if (mTimers.empty() || deadline < mTimers.top().deadline) {
                    needWake = true;
                }
                mTimers.push({deadline, h});
            }

            if (needWake) SetEvent(mWakeEvent);
        }

        detail::AutoHandle mWakeEvent = nullptr;
        detail::AutoHandle mTimerHandle = nullptr;
        std::atomic<bool>  mRunning = true;

        std::mutex                                                                         mMutex;
        std::priority_queue<TimerEntry, std::vector<TimerEntry>, std::greater<TimerEntry>> mTimers;
    };

} // namespace sapphire::coro