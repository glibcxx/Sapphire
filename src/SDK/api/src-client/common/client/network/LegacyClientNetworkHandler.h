#pragma once

#include "ClientNetworkHandler.h"
#include "SDK/api/src/common/world/level/LevelListener.h"

class LegacyClientNetworkHandler : public ClientNetworkHandler, public LevelListener {
};