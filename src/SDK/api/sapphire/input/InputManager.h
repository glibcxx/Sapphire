#pragma once

#include <Windows.h>
#include <winrt/Windows.UI.Core.h>
#include <bitset>

#include "../util/Singleton.h"
#include "SDK/api/src/common/world/phys/Vec2.h"

namespace sapphire::input {

    class InputInterceptor;

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

    class InputManager : public sapphire::Singleton<InputManager> {
    public:
        class MouseFeedHook;

        // 按键按下的瞬间
        bool isKeyDown(KeyCode key) const;
        // 按键抬起的瞬间
        bool isKeyUp(KeyCode key) const;
        // 按键按住状态
        bool isKeyPressed(KeyCode key) const;

        Vec2           getMousePosition() const;
        Vec2           getMouseDelta() const;
        MouseWheelData getMouseWheelData() const;

        using CoreWindow = winrt::Windows::UI::Core::CoreWindow;

        InputManager(CoreWindow &coreWindow);

        ~InputManager();

    private:
        using CoreDispatcher = winrt::Windows::UI::Core::CoreDispatcher;
        using PointerEventArgs = winrt::Windows::UI::Core::PointerEventArgs;
        using AcceleratorKeyEventArgs = winrt::Windows::UI::Core::AcceleratorKeyEventArgs;
        using CharacterReceivedEventArgs = winrt::Windows::UI::Core::CharacterReceivedEventArgs;

        void init();

        void onAcceleratorKeyActivated(const CoreDispatcher &sender, const AcceleratorKeyEventArgs &args);

        void onCharacterReceived(const CoreWindow &sender, const CharacterReceivedEventArgs &args);

        void onPointerPressed(const CoreWindow &sender, const PointerEventArgs &args);

        void onPointerReleased(const CoreWindow &sender, const PointerEventArgs &args);

        void onPointerWheelChanged(const CoreWindow &sender, const PointerEventArgs &args);

        Vec2           mMousePosition{};
        Vec2           mPreviousMousePosition{};
        Vec2           mMouseDelta{};
        MouseWheelData mMouseWheel{};

        std::reference_wrapper<InputInterceptor> mInputInterceptor;

        // --- State variables ---
        std::map<KeyCode, bool> mCurrentKeyStates;
        std::map<KeyCode, bool> mPreviousKeyStates;

        mutable std::mutex mStateMutex;

        // --- Internal API (called by hooks) ---
        void _onRawKeyEvent(KeyCode key, bool is_down);
        void _onRawMouseButtonEvent(KeyCode button, bool is_down, const Vec2 &pos);
        void _onRawMouseMove(const Vec2 &pos);
        void _onRawMouseWheel(float dx, float dy);

        // Called at the end of a frame to shift states
        void _onFrameEnd();

        CoreWindow     mCoreWindow{nullptr};
        CoreDispatcher mCoreDispatcher{nullptr};

        CoreWindow::PointerPressed_revoker              mPointerPressedRevoker;
        CoreWindow::PointerReleased_revoker             mPointerReleasedRevoker;
        CoreWindow::PointerWheelChanged_revoker         mPointerWheelRevoker;
        CoreDispatcher::AcceleratorKeyActivated_revoker mAcceleratorRevoker;
        CoreWindow::CharacterReceived_revoker           mCharacterReceivedRevoker;
    };

    class InputInterceptor : public sapphire::Singleton<InputInterceptor> {
        enum MouseBlockStatus : uint8_t {
            NoBlock = 0,
            BlockMouseMove = 1 << 0,
            BlockMouseButton = 1 << 1,
            BlockMouseWheel = 1 << 2,
            BlockAll = BlockMouseMove | BlockMouseButton | BlockMouseWheel,
        };

    public:
        /**
         * @brief 请求在当前帧拦截一个或多个特定的按键。
         * @param key 要拦截的按键。
         */
        void requestKeyBlock(KeyCode key);

        /**
         * @brief 请求在当前帧拦截所有鼠标的按键。
         */
        void requestAllMouseButtonBlock() { mBlockMouseStatus |= MouseBlockStatus::BlockMouseButton; }

        /**
         * @brief 请求在当前帧拦截鼠标的移动。
         */
        void requestMouseMoveBlock() { mBlockMouseStatus |= MouseBlockStatus::BlockMouseMove; }

        /**
         * @brief 请求在当前帧拦截鼠标滚轮的滚动。
         */
        void requestMouseWheelBlock() {
            mBlockMouseStatus |= MouseBlockStatus::BlockMouseWheel;
        }

        /**
         * @brief 请求在当前帧拦截所有的键盘输入。
         *        这通常用于ImGui窗口获得键盘焦点时。
         */
        void requestAllKeyboardBlock() { mBlockAllKeys = true; }

        /**
         * @brief 请求在当前帧拦截所有的鼠标输入（移动、点击、滚轮）。
         *        这通常用于ImGui窗口获得鼠标焦点时。
         */
        void requestAllMouseInputBlock() { mBlockMouseStatus |= MouseBlockStatus::BlockAll; }

        /**
         * @brief 请求在当前帧拦截所有的输入。
         */
        void requestAllInputBlock() {
            requestAllKeyboardBlock();
            requestAllMouseInputBlock();
        }

    private:
        friend class GuiOverlay;
        friend class InputManager;

        bool isKeyBlocked(KeyCode key) const;
        bool isMouseButtonBlocked() const;
        bool isMouseMoveBlocked() const;
        bool isMouseWheelBlocked() const;
        bool isAllMouseInputBlocked() const;

        void refresh();

        static constexpr size_t keycodeToIndex(KeyCode key) {
            return static_cast<size_t>(key) - static_cast<size_t>(KeyCode::NamedKey_BEGIN);
        }

        std::atomic<bool>    mBlockAllKeys = false;                         // 键盘
        std::atomic<uint8_t> mBlockMouseStatus = MouseBlockStatus::NoBlock; // 鼠标

        static constexpr size_t KEY_CODE_COUNT =
            static_cast<size_t>(KeyCode::NamedKey_END) - static_cast<size_t>(KeyCode::NamedKey_BEGIN);

        // 不包含修饰键
        std::bitset<KEY_CODE_COUNT> mBlockedKeysAndButtons;

        mutable std::mutex mMutex;
    };

} // namespace sapphire::input
