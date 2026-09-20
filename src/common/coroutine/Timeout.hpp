#pragma once

#include "Awaitable.hpp"
#include "common/Expected.hpp"
#include "IoContext.hpp"
#include <system_error>

namespace sapphire::coro {

    enum WaitingState {
        Waiting = 0,
        Timeout,
        Finished,
    };

    template <typename InnerAwaiter>
    class TimeoutAwaiter : public detail::TimerNode {
        InnerAwaiter mInnerOperation;
        IoContext   &mIO;

        std::atomic<WaitingState> mState{};

    public:
        TimeoutAwaiter(InnerAwaiter &&op, IoContext &ctx, TimerNode::TimePoint t) noexcept :
            mInnerOperation(std::forward<InnerAwaiter>(op)), mIO(ctx) {
            expireTime = t;
        }

        constexpr bool await_ready() noexcept(noexcept(mInnerOperation.await_ready())) {
            return mInnerOperation.await_ready();
        }

        template <typename T>
        auto await_suspend(std::coroutine_handle<T> caller) noexcept(noexcept(mInnerOperation.await_suspend(caller))) {
            this->onExpire = [](TimerNode *base) noexcept {
                TimeoutAwaiter *self = static_cast<TimeoutAwaiter *>(base);

                WaitingState expected = WaitingState::Waiting;
                if (self->mState.compare_exchange_strong(expected, WaitingState::Timeout, std::memory_order_acq_rel)) {
                    self->mInnerOperation.cancelOperation(self->mInnerOperation);
                }
            };
            mIO.addTimer(this);
            return mInnerOperation.await_suspend(caller);
        }

        bool await_resume()
            requires std::same_as<void, decltype(mInnerOperation.await_resume())>
        {
            WaitingState expected = WaitingState::Waiting;
            if (mState.compare_exchange_strong(expected, WaitingState::Finished, std::memory_order_acq_rel)) {
                mIO.cancelTimer(this);
            } else if (expected == WaitingState::Timeout) {
                return false;
            }
            mInnerOperation.await_resume();
            return true;
        }

        Expected<decltype(mInnerOperation.await_resume()), std::errc> await_resume()
            requires(!std::same_as<void, decltype(mInnerOperation.await_resume())>)
        {
            WaitingState expected = WaitingState::Waiting;
            if (mState.compare_exchange_strong(expected, WaitingState::Finished, std::memory_order_acq_rel)) {
                mIO.cancelTimer(this);
            } else if (expected == WaitingState::Timeout) {
                return Unexpected{std::errc::timed_out};
            }
            return mInnerOperation.await_resume();
        }
    };

    template <typename Op, typename Rep, typename Period>
        requires requires(AwaitableTraits<Op>::AwaiterType &a) { AwaitableTraits<Op>::AwaiterType::cancelOperation(a); }
    inline TimeoutAwaiter<typename AwaitableTraits<Op>::AwaiterType>
    timeout(Op &&op, std::chrono::duration<Rep, Period> d, IoContext &ctx) noexcept {
        return {detail::getAwaiter(std::forward<Op>(op)), ctx, detail::TimerNode::Clock::now() + d};
    }

} // namespace sapphire::coro