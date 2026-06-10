#pragma once

#include "pch.h" // IWYU pragma: keep

class MinecraftCommands;

// size: 8
class DeferredCommandBase {
public:
    // vtb+0
    virtual ~DeferredCommandBase() = default;

    // vtb+1
    virtual void execute(MinecraftCommands &) = 0;
};