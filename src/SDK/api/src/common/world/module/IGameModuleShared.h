#pragma once

class GameModuleServer;
class IInPackagePacks;
class ServerInstanceEventCoordinator;

// size: 8
class IGameModuleShared {
public:
    // vtb+0
    virtual ~IGameModuleShared();

    // vtb+1
    virtual std::unique_ptr<GameModuleServer> createGameModuleServer() = 0;

    // vtb+2
    virtual std::shared_ptr<IInPackagePacks> createInPackagePacks() = 0;

    // vtb+3
    virtual void registerMolangQueries() = 0;

    // vtb+4
    virtual void registerServerInstanceHandler(ServerInstanceEventCoordinator &) = 0;
};