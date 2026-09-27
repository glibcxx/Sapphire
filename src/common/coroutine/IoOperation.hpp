#pragma once

#include <coroutine>
#include <concepts>
#include <stop_token>
#include <optional>
#include <utility>
#include <winerror.h>

#include "common/sys/MiniWindows.h"
#include "detail/WinDefs.h"

namespace sapphire::coro {

    struct IoOperation : public win::overlapped {
        std::coroutine_handle<> mContinuation = nullptr;

        sys::win::dword_t mBytesTransferred = 0;
        sys::win::dword_t mErrorCode = 0;

        constexpr IoOperation() noexcept {
            Internal = 0;
            InternalHigh = 0;
            Offset = 0;
            OffsetHigh = 0;
            hEvent = 0;
        }

        LPOVERLAPPED asOverlapped() noexcept {
            return reinterpret_cast<LPOVERLAPPED>(static_cast<win::overlapped *>(this));
        }
    };

    template <typename BaseOperation>
        requires std::derived_from<BaseOperation, coro::IoOperation>
              && requires(BaseOperation &base) { {BaseOperation::cancelOperation(base) } noexcept -> std::same_as<void>; }
    struct [[nodiscard]] CancelableOperation : public BaseOperation {
        using CancelCallbackType = void (*)(BaseOperation &self) noexcept;
        struct TinyCancelCallback {
            BaseOperation     *mThis;
            CancelCallbackType mCancel;

            void operator()() const noexcept { mCancel(*mThis); }
        };

        std::stop_token                                       mCancelToken;
        std::optional<std::stop_callback<TinyCancelCallback>> mCancelCallback;
        template <typename... Args>
        CancelableOperation(std::stop_token &&cancelToken, Args &&...args) noexcept(noexcept(BaseOperation(std::forward<Args>(args)...))) :
            BaseOperation(std::forward<Args>(args)...), mCancelToken(std::move(cancelToken)) {}
        CancelableOperation(CancelableOperation &&other) noexcept :
            BaseOperation(std::move(other)), mCancelToken(std::move(other.mCancelToken)) {}
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

} // namespace sapphire::coro