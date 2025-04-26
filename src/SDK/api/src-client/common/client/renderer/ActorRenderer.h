#pragma once

#include <cstdint>

// Guessed name
class ActorRenderer {
public:
    void **vtable;

    ActorRenderer() = delete;

    // vtb+0
    SDK_VAPI ~ActorRenderer();

    // vtb+1
    SDK_VAPI uint64_t unk_virtual_1(float *a2, uint64_t a3);
};