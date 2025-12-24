#pragma once

#include "SDK/api/src/common/world/events/ClientInstanceEventListener.h"

namespace sapphire::event::impl {

    class SapphireClientInstanceEventListener : public ClientInstanceEventListener {
    public:
        SapphireClientInstanceEventListener(ClientInstance &client);

        virtual EventResult onClientEnteredWorld(ClientInstance &instance) override;

        virtual EventResult onStartLeaveGame(ClientInstance &instance) override;
    };

} // namespace sapphire::event::impl