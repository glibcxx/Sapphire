#pragma once

enum class InputMode : uint32_t {
    Undefined = 0,
    Mouse = 1,
    Touch = 2,
    GamePad = 3,
    MotionController = 4,
    Count = 5,
};

enum class InputBindingMode : uint32_t {
    Undefined = 0,
    MouseAndKeyboard = 1,
    Gamepad = 2,
    MotionController = 3,
};

enum class HoloUIInputMode : uint32_t {
    Unknown = 0,
    Controller = 1,
    Gaze = 2,
    Mouse = 3,
};

enum class NewInteractionModel : int {
    Touch = 0,
    Crosshair = 1,
    Classic = 2,
    Count = 3,
};
