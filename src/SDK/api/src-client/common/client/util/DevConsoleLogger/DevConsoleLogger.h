#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src-deps/Core/File/Path.h"
#include "SDK/api/src-deps/Core/File/FileStream.h"
#include "SDK/api/src/common/util/TimerFacade.h"

// size: 840
class DevConsoleLogger : public Bedrock::EnableNonOwnerReferences {
public:
    Core::HeapPathBuffer         mLogFolder;               // off+0
    Core::OutputFileStream       mLogFile;                 // off+32
    std::stringstream            mFileBuffer;              // off+328
    std::string                  mTimeStamp;               // off+576
    std::function<std::string()> mGetTestrunIDCallback;    // off+608
    std::function<bool()>        mIsAutomationRunCallback; // off+672
    TimerFacade                  mTimerFacade;             // off+736

    DevConsoleLogger *ctor(); // \x40\x53\x55\x56\x57\x41\x54\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x49\x8B\xF9 (1.21.50)
};