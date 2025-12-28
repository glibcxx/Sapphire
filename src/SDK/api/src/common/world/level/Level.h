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
    SDK_API Level(/* FIXME: args */);
#pragma SDK_LINKER_SYM_ALIAS("??0Level@@AEAA@XZ", "?ctor@Level@@AEAAPEAV1@XZ")

    SPHR_DECL_API("1.21.2", "\x48\x8B\xC4\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x44\x0F\x29\x40\x00\x44\x0F\x29\x88\x00\x00\x00\x00\x44\x0F\x29\x90\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4D\x8B\xE1\x4D\x8B\xF8")
    SPHR_DECL_API("1.21.50,1.21.60", "disp:13,call", "\x4C\x8B\xC6\x48\x8D\x95\x00\x00\x00\x00\x48\x8B\xCF\xE8\x00\x00\x00\x00\x90")
    SDK_API Level *ctor(/* FIXME: args */);

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x8B\x4B\x00\x48\x85\xC9\x74\x00\x48\x8B\x41")
    SDK_API /*virtual*/ void tick() /*override*/;

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x8B\x07\x48\x8B\xCF\x48\x8B\x80\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x48\x8B\x50")
    SDK_API /*virtual*/ void _subtick() /*override*/;

    /*virtual*/ ServerPlayerEventCoordinator &getServerPlayerEventCoordinator() /*override*/ {
        return memory::vCall<ServerPlayerEventCoordinator &>(this, vCall_getServerPlayerEventCoordinator);
    }

    /*virtual*/ ClientPlayerEventCoordinator &getClientPlayerEventCoordinator() /*override*/ {
        return memory::vCall<ClientPlayerEventCoordinator &>(this, vCall_getServerPlayerEventCoordinator + 1);
    }
};