#include "EventAtClientInstanceEventListener.h"
#include "SDK/api/sapphire/event/EventBus.h"
#include "SDK/api/src-client/common/client/game/ClientInstance.h"
#include "SDK/api/src/common/world/events/ClientInstanceEventCoordinator.h"
#include "../EnterWorldEvent.h"
#include "../LeaveWorldEvent.h"

namespace sapphire::event::impl {

    SapphireClientInstanceEventListener::SapphireClientInstanceEventListener(ClientInstance &client) {
        client.mEventCoordinator->registerListener(this);
    }

    EventResult SapphireClientInstanceEventListener::onClientEnteredWorld(ClientInstance &instance) {
        EventBus::getInstance().dispatchEvent(EnterWorldEvent{instance});
        return EventResult::KeepGoing;
    }

    EventResult SapphireClientInstanceEventListener::onStartLeaveGame(ClientInstance &instance) {
        EventBus::getInstance().dispatchEvent(LeaveWorldEvent{instance});
        return EventResult::KeepGoing;
    }

} // namespace sapphire::event::impl