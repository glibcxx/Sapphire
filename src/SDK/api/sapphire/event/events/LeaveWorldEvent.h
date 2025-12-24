#pragma once

#include "../Event.h"
#include "SDK/api/src-client/common/client/game/IClientInstance.h"

namespace sapphire::event {

    class LeaveWorldEvent : public Event {
    public:
        IClientInstance &mClientInstance;

        LeaveWorldEvent(IClientInstance &client) : mClientInstance(client) {}
    };

} // namespace sapphire::event