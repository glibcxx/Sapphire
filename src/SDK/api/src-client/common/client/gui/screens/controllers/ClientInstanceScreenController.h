#pragma once

#include "MinecraftScreenController.h"

class ClientInstanceScreenModel;
class PlatformMultiplayerRestrictions;

// size: 3128 (1.21.2), 3136 (1.21.50), 3264 (1.21.60)
class ClientInstanceScreenController : public MinecraftScreenController {
public:
    std::shared_ptr<ClientInstanceScreenModel>       mClientInstanceScreenModel;       // off+3112
    std::unique_ptr<PlatformMultiplayerRestrictions> mPlatformMultiplayerRestrictions; // off+3128
};