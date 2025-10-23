#pragma once

#include "SDK/api/src-deps/Core/Threading/Async.h"

class ContentIdentity;
class LevelSettings;
struct LevelSummary;

enum class StartIntent : int {
    JoinWorld = 0,
    UploadToRealm = 1,
};

// size: 8
class IGameServerStartup {
public:
    // vtb+0
    virtual ~IGameServerStartup() = default;

    // vtb+1
    virtual bool canStartLocalServer() const = 0;

    // vtb+2
    virtual Bedrock::Threading::IAsyncResult<void>::Handle
    startLocalServerAsyncFromSummary(const LevelSummary &levelSummary) = 0;

    // vtb+3
    virtual Bedrock::Threading::IAsyncResult<void>::Handle startLocalServerAsync(
        const std::string     &levelId,
        const std::string     &levelName,
        const ContentIdentity &premiumTemplateContentIdentity,
        const LevelSettings   &settings,
        StartIntent            startIntent
    ) = 0;

    // vtb+4
    virtual bool startLocalServerBlocking(
        const std::string     &levelId,
        const std::string     &levelName,
        const ContentIdentity &premiumTemplateContentIdentity,
        const LevelSettings   &settings,
        StartIntent            startIntent
    ) = 0;

    // vtb+5
    virtual bool isHostingLocalDedicatedServer() const = 0;
};