#pragma once

#include "interface/OreUIITelemetry.h"

class IClientInstance;
class IMinecraftEventing;

namespace OreUI {

    // size: 24
    class Telemetry : public ITelemetry {
    public:
        IClientInstance    &mClientInstance; // off+8
        IMinecraftEventing &mEventing;       // off+16
    };

} // namespace OreUI