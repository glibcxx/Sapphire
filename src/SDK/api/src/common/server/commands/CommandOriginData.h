#pragma once

#include "SDK/api/src-deps/Core/Utility/UUID.h"

enum class CommandOriginType : uint8_t {
    Player = 0,
    CommandBlock = 1,
    MinecartCommandBlock = 2,
    DevConsole = 3,
    Test = 4,
    AutomationPlayer = 5,
    ClientAutomation = 6,
    DedicatedServer = 7,
    Entity = 8,
    Virtual = 9,
    GameArgument = 10,
    EntityServer = 11,
    Precompiled = 12,
    GameDirectorEntityServer = 13,
    Scripting = 14,
    ExecuteContext = 15,
};

// size: 64
struct CommandOriginData {
    CommandOriginType mType;      // off+0
    mce::UUID         mUUID;      // off+8
    std::string       mRequestId; // off+24
    int64_t           mPlayerId;  // off+56
};