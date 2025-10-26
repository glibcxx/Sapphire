#pragma once

#include "SDK/api/src-external/RenderDragon/Rendering/LightingModels.h"
#include "IAdvancedGraphicsHardwareOptions.h"

// size: 24
class IAdvancedGraphicsOptions : public IAdvancedGraphicsHardwareOptions {
public:
    // vtb+0
    virtual ~IAdvancedGraphicsOptions() override;

    // vtb+5
    virtual bool areRayTracingResourcesAvailable() const = 0;

    // vtb+6
    virtual bool arePBRResourcesAvailable() const = 0;

    // vtb+7
    virtual bool isRayTracingAvailable() const = 0;

    // vtb+8
    virtual bool isDeferredShadingAvailable() const = 0;

    // vtb+9
    virtual dragon::rendering::LightingModels getLightingModel() const = 0;

    // vtb+10
    virtual void setLightingModel(const dragon::rendering::LightingModels) = 0;
};