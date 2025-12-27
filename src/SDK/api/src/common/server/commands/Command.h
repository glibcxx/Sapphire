#pragma once

#include "CommandFlag.h"
#include "CommandPermissionLevel.h"
#include <entt/locator/locator.hpp>

class CommandRegistry;
class CommandOrigin;
class CommandOutput;

// size: 32
class Command {
public:
    int                    mVersion = 0;                                        // off+8
    const CommandRegistry *mRegistry = nullptr;                                 // off+16
    int                    mCommandSymbol = -1;                                 // off+24
    CommandPermissionLevel mPermissionLevel = CommandPermissionLevel::Internal; // off+28
    CommandFlag            mFlags;                                              // off+30

    Command() = default;

    // vtb+0
    virtual ~Command();

    // vtb+1
    virtual bool collectOptionalArguments() { return true; }

    // vtb+2
    virtual void execute(const CommandOrigin &, CommandOutput &) const = 0;
};
