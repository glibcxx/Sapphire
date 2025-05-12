#include "EventManager.h"

ListenerId EventManager::sNextListenerId = 0;

EventManager &EventManager::getInstance() {
    static EventManager instance;
    return instance;
}
