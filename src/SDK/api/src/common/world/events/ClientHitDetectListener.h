#pragma once

#include "EventResult.h"
#include "SDK/api/src/common/world/phys/HitResult.h"

// size: 8
class ClientHitDetectListener {
public:
    // vtb+0
    virtual ~ClientHitDetectListener() = default;

    // vtb+1
    virtual EventResult onChangedHitResult(HitResult &hitResult) { return EventResult::KeepGoing; }

    // vtb+2
    virtual EventResult onContinuousHitResult(HitResult &hitResult) { return EventResult::KeepGoing; }

    // vtb+3
    virtual EventResult onChangedPickHitResult(HitResult &hitResult) { return EventResult::KeepGoing; }

    // vtb+4
    virtual EventResult onContinuousPickHitResult(HitResult &hitResult) { return EventResult::KeepGoing; }
};
