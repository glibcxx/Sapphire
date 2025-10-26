#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"

// size: 24
class IAdvancedGraphicsHardwareOptions : public Bedrock::EnableNonOwnerReferences {
public:
    // vtb+0
    virtual ~IAdvancedGraphicsHardwareOptions() override;

    // vtb+1
    virtual bool isHardwareRayTracingCompatible() const = 0;

    // vtb+2
    virtual bool isHardwareDeferredCompatible() const = 0;

    // vtb+3
    virtual bool isPlatformCompatible() const = 0;

    // vtb+4
    virtual bool isUpscalingAvailable() const = 0;
};