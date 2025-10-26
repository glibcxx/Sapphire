#pragma once

#include "SDK/api/src-deps/Core/Debug/Log/ContentLogEndPoint.h"

// size: 664
class GuiContentLogEndPoint : public ContentLogEndPoint {
public:
    bool                                     mEnabled;     // off+32
    std::optional<LogLevel>                  mGuiLogLevel; // off+40
    moodycamel::ConcurrentQueue<std::string> mMessages;    // off+48
};