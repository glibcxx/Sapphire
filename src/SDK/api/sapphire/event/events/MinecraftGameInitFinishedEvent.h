#pragma once

#include "../Event.h"
#include "SDK/api/src-client/common/client/game/MinecraftGame.h"

namespace sapphire::event {

    class MinecraftGameInitFinishedEvent : public Event {
    public:
        MinecraftGame   &mMinecraftGame;
        IClientInstance &mClientInstance;

        MinecraftGameInitFinishedEvent(MinecraftGame &mg, IClientInstance &client) :
            mMinecraftGame(mg), mClientInstance(client) {}
    };

} // namespace sapphire::event