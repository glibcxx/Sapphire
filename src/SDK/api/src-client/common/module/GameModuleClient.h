#pragma once

#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"

class IClientInstance;
class ILevel;
class BaseGameVersion;
class Experiments;
class IResourcePackRepository;
class ResourcePackStack;
class ClientInputMappingFactory;
class CommandRegistry;

// size: 8
class GameModuleClient {
public:
    enum class ResourceLoadingPhase : int {
        AppStartup = 0,
        Frontend = 1,
        InGame = 2,
    };

    // vtb+0
    virtual ~GameModuleClient() = default;

    // vtb+1
    virtual void init(IClientInstance &client, Bedrock::NotNullNonOwnerPtr<ILevel> level) = 0;

    // vtb+2
    virtual void configureLevel(
        IClientInstance                    &client,
        Bedrock::NotNullNonOwnerPtr<ILevel> level,
        const Experiments                  &experiments,
        const BaseGameVersion              &version
    ) = 0;

    // vtb+3
    virtual void unk_virtual_3(IClientInstance &client) = 0;

    // vtb+4
    virtual void initializeResourceStack(
        const Experiments                                   *experiments,
        Bedrock::NotNullNonOwnerPtr<IResourcePackRepository> repo,
        ResourcePackStack                                   &stack,
        const BaseGameVersion                               &baseGameVersion,
        ResourceLoadingPhase                                 loadingPhase,
        bool                                                 a6
    ) = 0;

    // vtb+5
    virtual void unk_virtual_5(uintptr_t a1, std::weak_ptr<void> a2) = 0;

    // vtb+6
    virtual void tick() = 0;

    // vtb+7
    virtual void setupStandardCommands(CommandRegistry &registry) = 0;

    // vtb+8
    virtual void setupStartMenuScreenCommands(CommandRegistry &registry) = 0;

    // vtb+9
    virtual void registerActorRenderers(Bedrock::NotNullNonOwnerPtr<IClientInstance> client) = 0;

    // vtb+10
    virtual std::unique_ptr<ClientInputMappingFactory> createInputMappingFactory(IClientInstance &client) = 0;
};