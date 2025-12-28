#pragma once

#include "SDK/api/src-deps/Core/Utility/StringHash.h"
#include "SDK/api/src/common/server/commands/CommandPermissionLevel.h"

class CommandRegistry;
class CommandOutputSender;
class Minecraft;
class Command;
class DeferredCommandBase;

// size: 200
class MinecraftCommands {
public:
    std::unique_ptr<CommandOutputSender>                       mOutputSender;               // off+8
    std::unique_ptr<CommandRegistry>                           mRegistry;                   // off+16
    CommandPermissionLevel                                     mOpPermissionLevel;          // off+24
    Minecraft                                                 &mMinecraft;                  // off+32
    std::function<bool()>                                      mChatPermissionsCallback;    // off+40
    std::unordered_map<HashedString, std::unique_ptr<Command>> mCompiledCommandMap;         // off+104
    std::vector<std::unique_ptr<DeferredCommandBase>>          mDeferredCommands;           // off+168
    size_t                                                     mDeferredScriptCommandCount; // off+192

    MinecraftCommands(Minecraft &minecraft);

    // vtb+0
    virtual ~MinecraftCommands();
};