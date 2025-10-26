#pragma once

#include "Async.h"
#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"

namespace Bedrock::Threading {

    namespace Details {

        enum class ConditionalState : int {
            Pending = 0,
            Checking = 1,
            CancelPending = 2,
            Canceled = 3,
            Completed = 4,
        };

        // size: 200
        class PendingConditional : public Bedrock::Threading::IAsyncResult<void> {
        public:
            std::function<bool()> mCondition; // off+24
            std::mutex            mLock;      // off+88
            ConditionalState      mState;     // off+168

            std::vector<std::function<void(const Bedrock::Threading::IAsyncResult<void> &)>> mCallbacks; // off+176
        };

    } // namespace Details

    // size: 128
    class PendingConditionals : public Bedrock::EnableNonOwnerReferences {
    public:
        std::mutex                                                mLock;         // off+16
        std::vector<std::shared_ptr<Details::PendingConditional>> mConditionals; // off+96
    };

} // namespace Bedrock::Threading
