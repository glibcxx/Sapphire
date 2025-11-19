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

    SDK_API ClientInputHandler *ctor(
        const Bedrock::NotNullNonOwnerPtr<IClientInstance> &client,
        InputHandler                                       &inputHandler,
        IGameModuleApp                                     &gameModuleApp
    );
};