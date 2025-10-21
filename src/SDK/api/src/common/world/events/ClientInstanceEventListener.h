#pragma once

#include "EventResult.h"

class ClientInstance;
class Level;
class Minecraft;
struct ClientInstanceNotificationEvent;

// size: 8
class ClientInstanceEventListener {
public:
    // vtb+0
    virtual ~ClientInstanceEventListener() = default;

    // vtb+1
    virtual EventResult onClientInitializeStart(ClientInstance &instance) { return EventResult::KeepGoing; }

    // vtb+2
    virtual EventResult onClientInitializeEnd(ClientInstance &instance) { return EventResult::KeepGoing; }

    // vtb+3
    virtual EventResult onClientMinecraftInitialized(ClientInstance &instance, Minecraft &minecraft) {
        return EventResult::KeepGoing;
    }

    // vtb+4
    virtual EventResult onClientCreatedLevel(ClientInstance &instance, Level &level) { return EventResult::KeepGoing; }

    // vtb+5
    virtual EventResult onClientUpdateStart(ClientInstance &instance) { return EventResult::KeepGoing; }

    // vtb+6
    virtual EventResult onClientUpdateEnd(ClientInstance &instance) { return EventResult::KeepGoing; }

    // vtb+7
    virtual EventResult onClientSuspend(ClientInstance &instance) { return EventResult::KeepGoing; }

    // vtb+8
    virtual EventResult onClientEnteredWorld(ClientInstance &instance) { return EventResult::KeepGoing; }

    // vtb+9
    virtual EventResult onStartLeaveGame(ClientInstance &instance) { return EventResult::KeepGoing; }

    // vtb+10
    virtual EventResult onEvent(const ClientInstanceNotificationEvent &) { return EventResult::KeepGoing; }
};