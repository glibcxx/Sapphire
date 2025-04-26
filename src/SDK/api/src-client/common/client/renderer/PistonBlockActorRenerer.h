#pragma once

#include "SDK/core/Core.h"
#include "BlockActorRenderer.h"

// Guessed name
class MovingBlockActorRenderer : public BlockActorRenderer {
public:
    // vtb+2
    SDK_VAPI uint64_t renderFrame(uint64_t a2, uint64_t a3);
};

// Guessed name
class PistonBlockActorRenderer : public BlockActorRenderer {
public:
    // vtb+2
    SDK_VAPI uint64_t renderFrame(uint64_t a2, uint64_t a3);
};
