#include "EventManager.h"

namespace sapphire::event {

    ListenerId EventManager::sNextListenerId = 0;

    EventManager &EventManager::getInstance() {
        static EventManager instance;
        return instance;
    }

} // namespace sapphire::event