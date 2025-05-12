#include "EventManager.h"

EventManager &EventManager::getInstance() {
    static EventManager instance;
    return instance;
}
