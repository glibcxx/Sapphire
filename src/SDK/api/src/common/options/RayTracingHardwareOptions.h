#pragma once

#include "pch.h" // IWYU pragma: keep

#include "IAdvancedGraphicsHardwareOptions.h"

// size: 32
class RayTracingHardwareOptions : public IAdvancedGraphicsHardwareOptions {
public:
    bool mHardwareRayTracingCompatible; // off+24
    bool mHardwareDeferredCompatible;   // off+25
};