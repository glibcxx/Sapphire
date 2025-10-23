#pragma once

#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"

class Minecraft;
class IGameModuleShared;
class FileArchiver;
namespace Automation {
    class AutomationClient;
}

// size: 8
class IMinecraftApp {
public:
    // vtb+0
    virtual ~IMinecraftApp();

    // vtb+1
    virtual Bedrock::NotNullNonOwnerPtr<Minecraft> getPrimaryMinecraft() = 0;

    // vtb+2
    virtual Bedrock::NotNullNonOwnerPtr<Automation::AutomationClient> getAutomationClient() const = 0;

    // vtb+3
    virtual bool isEduMode() const = 0;

    // vtb+4
    virtual bool isDedicatedServer() const = 0;

    // vtb+5
    virtual void onNetworkMaxPlayersChanged(uint32_t newMaxPlayerCount) = 0;

    // vtb+6
    virtual IGameModuleShared &getGameModule() = 0;

    // vtb+7
    virtual void requestServerShutdown(const std::string &) = 0;

    // vtb+8
    virtual Bedrock::NotNullNonOwnerPtr<FileArchiver> getFileArchiver() const = 0;
};
