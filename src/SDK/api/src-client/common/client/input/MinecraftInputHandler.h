#pragma once

#include "SDK/api/src-client/common/client/game/IConfigListener.h"
#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"

class InputHandler;
class MoveInputHandler;

class MinecraftInputHandler : public IConfigListener, public Bedrock::EnableNonOwnerReferences {
public:
    std::unique_ptr<InputHandler> mInputHandler; // off+32

    // vtb+2
    virtual void _registerInputHandlers() = 0;

    // vtb+3
    virtual void _registerDebugInputHandlers() = 0;

    // vtb+4
    virtual std::unique_ptr<MoveInputHandler> _createMoveInputHandler() = 0;
};