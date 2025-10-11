#pragma once

#include "EventCoordinator.h"
#include "ClientHitDetectListener.h"

// size: 112
class ClientHitDetectCoordinator : public EventCoordinator<ClientHitDetectListener> {
public:
    uint64_t mUnk88;  // off+88
    bool     mUnk96;  // off+96
    uint32_t mUnk100; // off+100
    bool     mUnk104; // off+104
    uint32_t mUnk108; // off+108

    // vtb+0
    virtual ~ClientHitDetectCoordinator() override;
};