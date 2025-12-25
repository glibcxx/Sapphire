#pragma once

#include "CrashDumpLogShared.h"

// size: 40
struct SerialWorkListLogEntry {
    std::string mTitle;
    float       mTime;
};

// size: 152 (1.21.50)
class SerialWorkList {
public:
    enum class WorkResult : int {
        Repeat = 0,
        Complete = 1,
    };

    using WorkFunction = std::function<WorkResult()>;

    // size: 96
    struct WorkItem {
        const char  *name;               // off+0
        WorkFunction func;               // off+8
        uint64_t     callCount;          // off+72
        float        maxSessionTime;     // off+80
        float        totalExclusiveTime; // off+84
        float        totalInclusiveTime; // off+88
    };

    std::string                         mSeriesTitle;            // off+0
    CrashDumpLogStringID                mCDStringID;             // off+32
    std::deque<WorkItem>                mModalWorkList;          // off+40
    std::vector<WorkItem>               mModalWorkLog;           // off+80
    std::vector<SerialWorkListLogEntry> mLogEntries;             // off+104
    double                              mModalWorkItemStartTime; // off+128
    double                              mSeriesStartTime;        // off+136
    bool                                mStepWatchdogEnabled;    // off+144
};