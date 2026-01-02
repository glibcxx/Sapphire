#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"

// size: 96
struct ScheduledCallback {
public:
    int64_t               mTime;                 // off+0
    std::weak_ptr<bool>   mExistenceTracker;     // off+8
    std::function<void()> mCallback;             // off+24
    bool                  mShouldCheckExistence; // off+88
};

// size: 1
class CompareScheduledCallback {
public:
    bool operator()(const ScheduledCallback &, const ScheduledCallback &);
};

class DateManager : public Bedrock::EnableNonOwnerReferences {
public:
    enum class TimeZoneType : int {
        Utc = 0,
        Local = 1,
    };

    static constexpr const int64_t INVALID_DATE_TIME = 4294967295;
    static constexpr const int32_t SECONDS_PER_MINUTE = 60;
    static constexpr const int32_t MINUTES_PER_HOUR = 60;
    static constexpr const int32_t HOURS_PER_DAY = 24;
    static constexpr const int32_t MONTHS_PER_YEAR = 12;
    static constexpr const int32_t DAYS_PER_LEAP_YEAR = 366;
    static constexpr const int32_t DAYS_PER_NON_LEAP_YEAR = 365;
    static constexpr const int32_t SECONDS_PER_HOUR = 3600;
    static constexpr const int32_t SECONDS_PER_DAY = 86400;
    static constexpr const int32_t EPOCH_YEAR = 1970;
    static constexpr const int32_t EPOCH_MAX_YEAR = 2038;
    static constexpr const int32_t EPOCH_MAX_MONTH = 0;
    static constexpr const int32_t EPOCH_MAX_DAY = 18;
    static constexpr const int32_t TM_YEAR_OFFSET = 1900;
    static constexpr const int32_t TM_MONTH_OFFSET = 1;

    uint32_t   mTimeScale;               // off+24
    int64_t    mRealTime;                // off+32
    int64_t    mTime;                    // off+40
    std::mutex mScheduledCallbacksMutex; // off+48
    std::priority_queue<
        ScheduledCallback,
        std::vector<ScheduledCallback>,
        CompareScheduledCallback>
         mScheduledCallbacks; // off+128
    bool mUnk128;             // off+160
};