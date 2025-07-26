#pragma once

#include "../Event.h"
#include "SDK/api/src-client/common/client/game/MinecraftGame.h"

namespace sapphire::event {

    class GameUpdateGraphicEvent : public Event {
    public:
        MinecraftGame   &mMinecraftGame;
        IClientInstance &mClientInstance;
        const Timer     &mTimer;

        GameUpdateGraphicEvent(MinecraftGame &minecraftGame, IClientInstance &client, const Timer &timer) :
            mMinecraftGame(minecraftGame), mClientInstance(client), mTimer(timer) {}
    };

} // namespace sapphire::event