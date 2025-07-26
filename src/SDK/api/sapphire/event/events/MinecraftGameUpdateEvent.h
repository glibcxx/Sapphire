#pragma once

#include "../Event.h"
#include "SDK/api/src-client/common/client/game/MinecraftGame.h"

namespace sapphire::event {

    class MinecraftGameUpdateEvent : public Event {
    public:
        MinecraftGame &mMinecraftGame;

        MinecraftGameUpdateEvent(MinecraftGame &minecraftGame) :
            mMinecraftGame(minecraftGame) {}
    };

}