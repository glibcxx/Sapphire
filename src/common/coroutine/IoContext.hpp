#pragma once

#include <array>
#include <atomic>
#include <chrono>
#include <concepts>
#include <coroutine>
#include <limits>
#include <mutex>
#include <optional>
#include <queue>
#include <stop_token>

#include <system_error>
#include <winerror.h>

#include "Constant.hpp"
#include "Handle.hpp"
#include "IoOperation.hpp"
#include "Sync.hpp"
#include "common/Expected.hpp"
#include "common/sys/MiniWindows.h"
#include "detail/WinDefs.h"

#pragma comment(lib, "ntdll.lib")
extern "C" unsigned long __stdcall RtlNtStatusToDosError(long Status);

namespace sapphire::coro {

    namespace detail {

        struct TimerNode {
            using Clock = std::chrono::steady_clock;
            using TimePoint = Clock::time_point;
            using CallbackType = void (*)(TimerNode *self) noexcept;

            TimePoint    expireTime;
            CallbackType onExpire = nullptr;

            constexpr TimerNode() noexcept = default;

            void cancel() noexcept { onExpire = nullptr; }

            bool operator>(const TimerNode &other) const noexcept { return expireTime > other.expireTime; }
        };

        struct TimerNodePred {
            bool operator()(TimerNode *lhs, TimerNode *rhs) const noexcept { return *lhs > *rhs; }
        };

        using TimerQueue = std::priority_queue<TimerNode *, std::vector<TimerNode *>, TimerNodePred>;

    } // namespace detail

    template <typename BaseOperation>
        requires std::derived_from<BaseOperation, coro::IoOperation>
              && requires(BaseOperation &base) { BaseOperation::cancelOperation(base); }
    struct [[nodiscard]] CancelableOperation : BaseOperation {
        using CancelCallbackType = void (*)(BaseOperation &self) noexcept;
        struct TinyCancelCallback {
            BaseOperation     *mThis;
            CancelCallbackType mCancel;

            void operator()() const noexcept { mCancel(*mThis); }
        };

        std::stop_token                                       mCancelToken;
        std::optional<std::stop_callback<TinyCancelCallback>> mCancelCallback;
        template <typename... Args>
        CancelableOperation(std::stop_token &&cancelToken, Args &&...args) noexcept :
            BaseOperation(std::forward<Args>(args)...), mCancelToken(std::move(cancelToken)) {}
        bool await_suspend(std::coroutine_handle<> h) noexcept(noexcept(BaseOperation::await_suspend(h))) {
            if (mCancelToken.stop_requested()) {
                this->mErrorCode = ERROR_OPERATION_ABORTED;
                return false;
            }
            mCancelCallback.emplace(mCancelToken, TinyCancelCallback{this, &BaseOperation::cancelOperation});
            return BaseOperation::await_suspend(h);
        }
        decltype(auto) await_resume() noexcept(noexcept(BaseOperation::await_resume())) {
            mCancelCallback.reset();
            return BaseOperation::await_resume();
        }
    };

    class alignas(CacheLineSize) IoContext {
        using Clock = std::chrono::steady_clock;
        using TimePoint = Clock::time_point;

        win32::Handle      mIocp;
        detail::mutex      mTimerMutex;
        detail::TimerQueue mTimers;
        std::atomic<bool>  mWakePending{false};
        std::atomic<bool>  mStopRequested = false;

        IoContext(sys::win::handle_t h) noexcept : mIocp(h) {}

        static constexpr sys::win::ulong_ptr_t WAKEUP_KEY = std::numeric_limits<sys::win::ulong_ptr_t>::max();

    public:
        IoContext(IoContext &&other) noexcept : mIocp(std::move(other.mIocp)) {}

        static Expected<IoContext, std::error_code> create(int concurrentThread = 0) {
            sys::win::handle_t h = CreateIoCompletionPort((void *)win32::Handle::INVALID_HANDLE, NULL, 0, concurrentThread);
            if (!h) return {unexpected, GetLastError(), std::system_category()};
            return IoContext{h};
        }

        std::error_code attach(sys::win::handle_t hFile) const {
            if (!mIocp.isValid()) return {};

            if (!CreateIoCompletionPort(hFile, mIocp.get(), 0, 0)) return {(int)GetLastError(), std::system_category()};
            return {};
        }

        struct [[nodiscard]] Awaiter {
            IoContext &mIO;
            Awaiter(IoContext &io) noexcept : mIO(io) {}
            constexpr bool await_ready() const noexcept { return false; }
            void           await_suspend(std::coroutine_handle<> h) noexcept {
                sys::win::bool_t ok =
                    PostQueuedCompletionStatus(mIO.mIocp.get(), 0, reinterpret_cast<sys::win::ulong_ptr_t>(h.address()), nullptr);
                if (!ok) {
                    // FIXME: need re-schedule?
                }
            }
            constexpr void await_resume() const noexcept {}
        };

        Awaiter schedule() noexcept { return Awaiter{*this}; }

        struct [[nodiscard]] AfterAwaiter : public detail::TimerNode {
            IoContext              &mIO;
            std::coroutine_handle<> mContinuation;

            AfterAwaiter(IoContext &io, TimerNode::TimePoint deadline) noexcept : mIO(io) {
                TimerNode::expireTime = deadline;
            }

            constexpr bool await_ready() const noexcept { return false; }
            void           await_suspend(std::coroutine_handle<> h) noexcept {
                mContinuation = h;
                this->onExpire = [](TimerNode *base) noexcept {
                    auto *self = static_cast<AfterAwaiter *>(base);
                    self->mContinuation.resume();
                };

                mIO.addTimer(this);
            }

            constexpr void await_resume() const noexcept {}
        };

        template <typename Rep, typename Period>
        auto scheduleAfter(const std::chrono::duration<Rep, Period> &delay) noexcept {
            return AfterAwaiter{*this, Clock::now() + delay};
        }

        Awaiter yield() noexcept { return schedule(); }

        bool processEvents() { return _processEvents(); }

        void stop() {
            bool alreadyRequested = mStopRequested.exchange(true, std::memory_order_release);
            if (!alreadyRequested) PostQueuedCompletionStatus(mIocp.get(), 0, 0, nullptr);
        }

        bool stopRequested() const { return mStopRequested.load(std::memory_order_acquire); }

        const win32::Handle &getNativeHandle() const { return mIocp; }

        void addTimer(detail::TimerNode *node) {
            bool needWake = true;
            {
                std::lock_guard lock(mTimerMutex);
                needWake = mTimers.empty() || node->expireTime < mTimers.top()->expireTime;
                mTimers.push(node);
            }

            if (needWake) { PostQueuedCompletionStatus(mIocp.get(), 0, WAKEUP_KEY, nullptr); }
        }

        void cancelTimer(detail::TimerNode *node) noexcept {
            std::lock_guard guard(mTimerMutex);
            node->cancel();
        }

    private:
        std::size_t handleTimer() {
            std::vector<detail::TimerNode *> expiredTimers;
            {
                std::lock_guard lock(mTimerMutex);
                auto            now = std::chrono::steady_clock::now();

                while (!mTimers.empty() && mTimers.top()->expireTime <= now) {
                    if (mTimers.top()->onExpire) expiredTimers.push_back(mTimers.top());
                    mTimers.pop();
                }
            }
            for (auto timer : expiredTimers) timer->onExpire(timer);
            return expiredTimers.size();
        }

        bool _processEvents() {
            if (stopRequested()) return false;

            sys::win::dword_t     bytes = 0;
            sys::win::ulong_ptr_t key = 0;
            win::lpOverlapped     overlapped = nullptr;

            std::array<win::overlapped_entry, 64> entries;

            while (true) {
                sys::win::dword_t timeoutMs = 0xFFFFFFFF;
                {
                    std::lock_guard lock(mTimerMutex);
                    if (!mTimers.empty()) {
                        auto now = std::chrono::steady_clock::now();
                        auto expire = mTimers.top()->expireTime;

                        if (expire <= now) {
                            timeoutMs = 0;
                        } else {
                            timeoutMs = static_cast<sys::win::dword_t>(
                                std::chrono::duration_cast<std::chrono::milliseconds>(expire - now).count()
                            );
                        }
                    }
                }
                sys::win::ulong_t count;
                sys::win::bool_t  ok =
                    GetQueuedCompletionStatusEx(mIocp.get(), (LPOVERLAPPED_ENTRY)entries.data(), entries.size(), &count, timeoutMs, 0);

                if (ok) {
                    for (size_t idx = 0; idx < count; ++idx) {
                        auto &entry = entries[idx];
                        overlapped = entry.lpOverlapped;
                        bytes = entry.dwNumberOfBytesTransferred;
                        key = entry.lpCompletionKey;
                        sys::win::lstatus_t status = (sys::win::lstatus_t)entry.Internal;
                        if (overlapped) {
                            auto *op = static_cast<IoOperation *>(overlapped);
                            op->mBytesTransferred = bytes;
                            op->mErrorCode = status ? ERROR_SUCCESS : RtlNtStatusToDosError(status);

                            op->mContinuation.resume();
                        }
                        if (key == WAKEUP_KEY) {
                            // handle timer
                        } else if (key) {
                            std::coroutine_handle<>::from_address(reinterpret_cast<void *>(key)).resume();
                        } else if (ok && stopRequested()) {
                            PostQueuedCompletionStatus(mIocp.get(), 0, 0, nullptr);
                            return false;
                        }
                        handleTimer();
                    }
                } else {
                    // just assume it is WAIT_TIMEOUT
                    // if (GetLastError() == WAIT_TIMEOUT) {
                    // }
                    handleTimer();
                }
            }
            return false;
        }
    };

} // namespace sapphire::coro