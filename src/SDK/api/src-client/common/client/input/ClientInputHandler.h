#pragma once

#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"

class IClientInstance;
class InputHandler;
class IGameModuleApp;
class ClientBindingFactory;
class ClientInputMappingFactory;
class ClientInputHandlerProxy;

// size: 128 (1.21.2), 136 (1.21.50/1.21.60)
class ClientInputHandler {
public:
    Bedrock::NotNullNonOwnerPtr<IClientInstance> mClient;         // off+0
    InputHandler                                &mInputHandler;   // off+24
    std::unique_ptr<ClientBindingFactory>        mBindingFactory; // off+32
    std::unique_ptr<ClientInputMappingFactory>   mMappingFactory; // off+40
    std::string                                  mLastGameMode;   // off+48
    std::string                                  mLastHoloUIMode; // off+80

    bool _fill[13]; // off+112

    std::unique_ptr<ClientInputHandlerProxy> mProxy; // off+128

    SDK_API ClientInputHandler(
        const Bedrock::NotNullNonOwnerPtr<IClientInstance> &client,
        InputHandler                                       &inputHandler,
        IGameModuleApp                                     &gameModuleApp
    );

    SAPPHIRE_API("1.21.2", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x4C\x89\x4C\x24\x00\x48\x8B\xF2")
    SAPPHIRE_API("1.21.50", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x4D\x8B\xE1\x48\x8B\xFA")
    SAPPHIRE_API("1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x4D\x8B\xE9\x48\x8B\xFA")
    SDK_API ClientInputHandler *ctor(
        const Bedrock::NotNullNonOwnerPtr<IClientInstance> &client,
        InputHandler                                       &inputHandler,
        IGameModuleApp                                     &gameModuleApp
    );
};