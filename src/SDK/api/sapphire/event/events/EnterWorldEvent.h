#pragma once

#include "../Event.h"
#include "SDK/api/src-client/common/client/game/IClientInstance.h"

namespace sapphire::event {

    class EnterWorldEvent  : public Event{
    public:
        IClientInstance &mClientInstance;

        EnterWorldEvent(IClientInstance &client) : mClientInstance(client) {}
    };

} // namespace sapphire::event