#pragma once

#include "pch.h" // IWYU pragma: keep

#include "EventCoordinator.h"
#include "PlayerEventListener.h"

class PlayerGameplayHandler;

class PlayerEventCoordinator : public EventCoordinator<PlayerEventListener> {
public:
    std::unique_ptr<PlayerGameplayHandler> mPlayerGameplayHandler; // off+88
};