#pragma once

#include "EventCoordinator.h"
#include "ClientHitDetectListener.h"

// size: 88
class ClientHitDetectCoordinator : public EventCoordinator<ClientHitDetectListener> {
public:
    // vtb+0
    virtual ~ClientHitDetectCoordinator() = default;
};