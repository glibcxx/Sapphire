#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src-deps/Core/Debug/Log/LogEndPoint.h"

enum class LogArea : uint32_t;
enum class LogLevel : uint32_t;

// size: 32
class ContentLogEndPoint : public Bedrock::EnableNonOwnerReferences, public Bedrock::LogEndPoint {
public:
    // vtb+0
    virtual ~ContentLogEndPoint() override;

    // vtb+1
    virtual void log(const LogArea, const LogLevel, const char *) = 0;
};