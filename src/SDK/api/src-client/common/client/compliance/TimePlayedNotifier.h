#pragma once

namespace Compliance {

    // size: 96
    struct TimePlayedNotifier {
        uint32_t                                               mLastNotificationTimeM;
        bool                                                   mUnk4;
        __int32                                                mUnk8;
        __int64                                                mUnk16;
        std::chrono::steady_clock::time_point                  mStartTime;
        std::function<std::chrono::steady_clock::time_point()> mGetTimeCallback;
    };

} // namespace Compliance