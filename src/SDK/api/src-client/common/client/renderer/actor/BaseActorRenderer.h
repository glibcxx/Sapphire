#pragma once

#include <cstdint>

class Actor;

class BaseActorRenderer {
public:
    void **vtable;

    BaseActorRenderer() = delete;

    // vtb+0
    SDK_VAPI ~BaseActorRenderer();

    // vtb+1
    SDK_VAPI uint64_t _getOverlayColor(Actor &, float) const;
};