#pragma once

#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"

class Minecraft;
class IGameModuleShared;
class FileArchiver;
class EDUSystems;
class IClientInstance;
namespace Automation {
    class AutomationClient;
}

// size: 8
class IMinecraftApp {
public:
    // vtb+0
    virtual ~IMinecraftApp();

    // vtb+1
    virtual Bedrock::NonOwnerPointer<EDUSystems> getEDUSystems() = 0;

    // vtb+2
    virtual const Bedrock::NonOwnerPointer<EDUSystems> getEDUSystems() const = 0;

    // vtb+3
    virtual Bedrock::NotNullNonOwnerPtr<IClientInstance> getPrimaryClientInstance() = 0;

    // vtb+4
    virtual Bedrock::NotNullNonOwnerPtr<void> unk_virtual_4();

    // vtb+5
    virtual bool isEduMode() const = 0;

    // vtb+6
    virtual bool isDedicatedServer() const = 0;

    // vtb+7
    virtual void onNetworkMaxPlayersChanged(uint32_t newMaxPlayerCount) = 0;

    // vtb+8
    virtual IGameModuleShared &getGameModule() = 0;

    // vtb+9
    virtual void requestServerShutdown(const std::string &message) = 0;

    // vtb+10
    virtual Bedrock::NotNullNonOwnerPtr<FileArchiver> getFileArchiver() const = 0;
};
