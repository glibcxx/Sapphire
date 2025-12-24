#pragma once

#include "ILevel.h"
#include "common/Memory.hpp"
#include "SDK/api/src/common/world/level/BlockSourceListener.h"
#include "SDK/api/src/common/world/level/IWorldRegistriesProvider.h"
#include "SDK/api/src/common/world/events/ServerPlayerEventCoordinator.h"
#include "SDK/api/src/common/world/events/ClientPlayerEventCoordinator.h"

class Level : public ILevel,
              public BlockSourceListener,
              public IWorldRegistriesProvider {
#if MC_VERSION == v1_21_2
    static constexpr size_t vCall_getServerPlayerEventCoordinator = 166;
#elif MC_VERSION == v1_21_50
    static constexpr size_t vCall_getServerPlayerEventCoordinator = 182;
#elif MC_VERSION == v1_21_60
    static constexpr size_t vCall_getServerPlayerEventCoordinator = 183;
#endif

public:
    /*virtual*/ ServerPlayerEventCoordinator &getServerPlayerEventCoordinator() /*override*/ {
        return memory::vCall<ServerPlayerEventCoordinator &>(this, vCall_getServerPlayerEventCoordinator);
    }

    /*virtual*/ ClientPlayerEventCoordinator &getClientPlayerEventCoordinator() /*override*/ {
        return memory::vCall<ClientPlayerEventCoordinator &>(this, vCall_getServerPlayerEventCoordinator + 1);
    }
};