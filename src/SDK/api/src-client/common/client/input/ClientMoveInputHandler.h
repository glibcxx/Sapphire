#pragma once

class InputHandler;
struct MoveInputState;

// size: 8
class ClientMoveInputHandler {
public:
    // vtb+0
    virtual ~ClientMoveInputHandler();

    // vtb+1
    virtual void registerInputHandlers(InputHandler &inputHandler);

    static void _registerButtonUpTracking(InputHandler &inputHandler, const std::string &buttonName, bool MoveInputState::*setting);

    static void _registerButtonStateTracking(InputHandler &inputHandler, const std::string &buttonName, bool MoveInputState::*setting);

    static void _registerButtonDownTracking(InputHandler &inputHandler, const std::string &buttonName, bool MoveInputState::*setting);
};