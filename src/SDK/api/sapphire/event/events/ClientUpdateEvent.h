#pragma once

#include "../Event.h"
#include "SDK/api/src-client/common/client/game/MinecraftGame.h"

namespace sapphire::event {

    class ClientUpdateEvent : public Event {
    public:
        MinecraftGame &mMinecraftGame;

        ClientUpdateEvent(MinecraftGame &minecraftGame) :
            mMinecraftGame(minecraftGame) {}
    };

} // namespace sapphire::event