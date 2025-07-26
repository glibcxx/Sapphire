#pragma once

#include "../Event.h"
#include "SDK/api/src-client/common/client/game/MinecraftGame.h"

namespace sapphire::event {

    class ClientUpdateEvent : public Event {
        MinecraftGame &mMinecraftGame;

    public:
        ClientUpdateEvent(MinecraftGame &minecraftGame) :
            mMinecraftGame(minecraftGame) {}
    };

}