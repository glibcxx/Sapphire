#pragma once

#include "EventCoordinator.h"
#include "ClientInstanceEventListener.h"

class ClientInstanceEventHandler;

// size: 96
class ClientInstanceEventCoordinator : public EventCoordinator<ClientInstanceEventListener> {
public:
    std::unique_ptr<ClientInstanceEventHandler> mClientInstanceEventHandler; // off+88
};