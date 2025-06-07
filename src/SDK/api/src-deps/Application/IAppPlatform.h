#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src-deps/Core/File/Path.h"

// size: 24
class IAppPlatform : public Bedrock::EnableNonOwnerReferences {
public:
    // vtb+0
    virtual ~IAppPlatform() = 0;

    // vtb+1
    virtual bool restartRequested() = 0;

    // vtb+2
    virtual Core::PathBuffer<std::string> getLoggingPath() const = 0;

    // vtb+3
    virtual bool isLowMemoryDevice() const = 0;

    // ...
};