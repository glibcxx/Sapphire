#pragma once

#include "pch.h" // IWYU pragma: keep

#include "common/DowncastImpl.h"
#include "SDK/api/src/common/world/phys/Vec2.h"

namespace winrt::Windows::UI::Core {
    struct CoreWindow;
}

namespace sapphire::input {

    // clang-format off
    // 复制自 ImGuiKey，便于与其直接转换
    enum class KeyCode : int {
        None = 0,

        NamedKey_BEGIN = 512,

        Tab = 512,
        LeftArrow, RightArrow,
        UpArrow,   DownArrow,
        PageUp,    PageDown,
        Home,      End,
        Insert,
        Delete,
        Backspace,
        Space,
        Enter,
        Escape,
        LeftCtrl,  LeftShift,  LeftAlt,  LeftSuper,
        RightCtrl, RightShift, RightAlt, RightSuper,
        Menu,
        _0, _1, _2, _3, _4, _5, _6, _7, _8, _9,
        A, B, C, D, E, F, G, H, I, J,
        K, L, M, N, O, P, Q, R, S, T,
        U, V, W, X, Y, Z,
        F1,  F2,  F3,  F4,  F5,  F6,
        F7,  F8,  F9,  F10, F11, F12,
        F13, F14, F15, F16, F17, F18,
        F19, F20, F21, F22, F23, F24,
        Apostrophe,        //  '
        Comma,             //  ,
        Minus,             //  -
        Period,            //  .
        Slash,             //  /
        Semicolon,         //  ;
        Equal,             //  =
        LeftBracket,       //  [
        Backslash,         // `\`
        RightBracket,      //  ]
        GraveAccent,       //  `
        CapsLock,
        ScrollLock,
        NumLock,
        PrintScreen,
        Pause,
        Keypad0, Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6, Keypad7, Keypad8, Keypad9,
        KeypadDecimal,     // .
        KeypadDivide,
        KeypadMultiply,
        KeypadSubtract,
        KeypadAdd,
        KeypadEnter,
        KeypadEqual,
        AppBack,               // Available on some keyboard/mouses. Often referred as "Browser Back"
        AppForward,
        Oem102,                // Non-US backslash.

        GamepadStart,          // Menu (Xbox)      + (Switch)   Start/Options (PS)
        GamepadBack,           // View (Xbox)      - (Switch)   Share (PS)
        GamepadFaceLeft,       // X (Xbox)         Y (Switch)   Square (PS)        // Tap: Toggle Menu. Hold: Windowing mode (Focus/Move/Resize windows)
        GamepadFaceRight,      // B (Xbox)         A (Switch)   Circle (PS)        // Cancel / Close / Exit
        GamepadFaceUp,         // Y (Xbox)         X (Switch)   Triangle (PS)      // Text Input / On-screen Keyboard
        GamepadFaceDown,       // A (Xbox)         B (Switch)   Cross (PS)         // Activate / Open / Toggle / Tweak
        GamepadDpadLeft,       // D-pad Left                                       // Move / Tweak / Resize Window (in Windowing mode)
        GamepadDpadRight,      // D-pad Right                                      // Move / Tweak / Resize Window (in Windowing mode)
        GamepadDpadUp,         // D-pad Up                                         // Move / Tweak / Resize Window (in Windowing mode)
        GamepadDpadDown,       // D-pad Down                                       // Move / Tweak / Resize Window (in Windowing mode)
        GamepadL1,             // L Bumper (Xbox)  L (Switch)   L1 (PS)            // Tweak Slower / Focus Previous (in Windowing mode)
        GamepadR1,             // R Bumper (Xbox)  R (Switch)   R1 (PS)            // Tweak Faster / Focus Next (in Windowing mode)
        GamepadL2,             // L Trig. (Xbox)   ZL (Switch)  L2 (PS) [Analog]
        GamepadR2,             // R Trig. (Xbox)   ZR (Switch)  R2 (PS) [Analog]
        GamepadL3,             // L Stick (Xbox)   L3 (Switch)  L3 (PS)
        GamepadR3,             // R Stick (Xbox)   R3 (Switch)  R3 (PS)
        GamepadLStickLeft,     // [Analog]                                         // Move Window (in Windowing mode)
        GamepadLStickRight,    // [Analog]                                         // Move Window (in Windowing mode)
        GamepadLStickUp,       // [Analog]                                         // Move Window (in Windowing mode)
        GamepadLStickDown,     // [Analog]                                         // Move Window (in Windowing mode)
        GamepadRStickLeft,     // [Analog]
        GamepadRStickRight,    // [Analog]
        GamepadRStickUp,       // [Analog]
        GamepadRStickDown,     // [Analog]

        MouseLeft, MouseRight, MouseMiddle,
        MouseX1, MouseX2,
        MouseWheelX, MouseWheelY,

        ReservedForModCtrl, ReservedForModShift, ReservedForModAlt, ReservedForModSuper,

        NamedKey_END,

        Mod_None                   = 0,
        Mod_Ctrl                   = 1 << 12, // Ctrl (non-macOS), Cmd (macOS)
        Mod_Shift                  = 1 << 13, // Shift
        Mod_Alt                    = 1 << 14, // Option/Menu
        Mod_Super                  = 1 << 15, // Windows/Super (non-macOS), Ctrl (macOS)
        Mod_Mask_                  = 0xF000,  // 4-bits
    };
    // clang-format on

    struct MouseWheelData {
        float horizontal = 0.0f; // (shift+wheel)
        float vertical = 0.0f;

        MouseWheelData() = default;
        MouseWheelData(float horizontal, float vertical) :
            horizontal(horizontal), vertical(vertical) {}
        MouseWheelData(const Vec2 &data) :
            horizontal(data.x), vertical(data.y) {}

        operator Vec2() const {
            return Vec2{horizontal, vertical};
        }
    };

    class InputManagerImpl;

    class InputManager : public DowncastImpl<InputManager, InputManagerImpl> {
    public:
        // 按键按下的瞬间
        SPHR_API bool isKeyDown(KeyCode key) const;
        // 按键抬起的瞬间
        SPHR_API bool isKeyUp(KeyCode key) const;
        // 按键按住状态
        SPHR_API bool isKeyPressed(KeyCode key) const;

        SPHR_API Vec2           getMousePosition() const;
        SPHR_API Vec2           getMouseDelta() const;
        SPHR_API MouseWheelData getMouseWheelData() const;

        SPHR_API bool isLMouseDoubleClicked() const;
        SPHR_API bool isRMouseDoubleClicked() const;

        void blockInput(bool block = true) { mBlockInput = block; }
        bool isInputBlocked() const { return mBlockInput; }

        using CoreWindow = winrt::Windows::UI::Core::CoreWindow;

        void init(CoreWindow &coreWindow);

        SPHR_API static InputManager &getInstance();

    protected:
        bool mBlockInput = false;
    };

} // namespace sapphire::input
