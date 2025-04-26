#pragma once

#include "ActorRenderer.h"

// Guessed name
class BlockActorRenderer : public ActorRenderer {
public:
    BlockActorRenderer() = delete;

    // vtb+0
    SDK_VAPI ~BlockActorRenderer();

    // vtb+2
    SDK_VAPI uint64_t renderFrame(uint64_t a2, uint64_t a3) /* = 0 */; // Guessed name
};