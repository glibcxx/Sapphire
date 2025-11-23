#include "EventBus.h"

namespace sapphire::event {

    ListenerId EventBus::sNextListenerId = 0;

    EventBus &EventBus::getInstance() {
        static EventBus instance;
        return instance;
    }

} // namespace sapphire::event