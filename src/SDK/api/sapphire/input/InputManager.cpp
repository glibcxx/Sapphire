#include "InputManager.h"

#include <winrt/Windows.UI.Input.h>
#include <winrt/Windows.Foundation.h>

#include "SDK/api/sapphire/hook/Hook.h"
#include "SDK/api/sapphire/GUI/GUI.h"
#include "SDK/api/src-deps/Input/MouseDevice.h"
#include "SDK/api/src-client/common/client/game/ClientInstance.h"
#include "util/String.hpp"

#include <imgui/imgui.h>

using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;

static constexpr ImGuiKey KeyEventToImGuiKey(WPARAM wParam) {
    switch (wParam) {
    case VK_TAB: return ImGuiKey_Tab;
    case VK_LEFT: return ImGuiKey_LeftArrow;
    case VK_RIGHT: return ImGuiKey_RightArrow;
    case VK_UP: return ImGuiKey_UpArrow;
    case VK_DOWN: return ImGuiKey_DownArrow;
    case VK_PRIOR: return ImGuiKey_PageUp;
    case VK_NEXT: return ImGuiKey_PageDown;
    case VK_HOME: return ImGuiKey_Home;
    case VK_END: return ImGuiKey_End;
    case VK_INSERT: return ImGuiKey_Insert;
    case VK_DELETE: return ImGuiKey_Delete;
    case VK_BACK: return ImGuiKey_Backspace;
    case VK_SPACE: return ImGuiKey_Space;
    case VK_RETURN: return ImGuiKey_Enter;
    case VK_ESCAPE: return ImGuiKey_Escape;
    case VK_OEM_7: return ImGuiKey_Apostrophe;
    case VK_OEM_COMMA: return ImGuiKey_Comma;
    case VK_OEM_MINUS: return ImGuiKey_Minus;
    case VK_OEM_PERIOD: return ImGuiKey_Period;
    case VK_OEM_2: return ImGuiKey_Slash;
    case VK_OEM_1: return ImGuiKey_Semicolon;
    case VK_OEM_PLUS: return ImGuiKey_Equal;
    case VK_OEM_4: return ImGuiKey_LeftBracket;
    case VK_OEM_5: return ImGuiKey_Backslash;
    case VK_OEM_6: return ImGuiKey_RightBracket;
    case VK_OEM_3: return ImGuiKey_GraveAccent;
    case VK_CAPITAL: return ImGuiKey_CapsLock;
    case VK_SCROLL: return ImGuiKey_ScrollLock;
    case VK_NUMLOCK: return ImGuiKey_NumLock;
    case VK_SNAPSHOT: return ImGuiKey_PrintScreen;
    case VK_PAUSE: return ImGuiKey_Pause;
    case VK_NUMPAD0: return ImGuiKey_Keypad0;
    case VK_NUMPAD1: return ImGuiKey_Keypad1;
    case VK_NUMPAD2: return ImGuiKey_Keypad2;
    case VK_NUMPAD3: return ImGuiKey_Keypad3;
    case VK_NUMPAD4: return ImGuiKey_Keypad4;
    case VK_NUMPAD5: return ImGuiKey_Keypad5;
    case VK_NUMPAD6: return ImGuiKey_Keypad6;
    case VK_NUMPAD7: return ImGuiKey_Keypad7;
    case VK_NUMPAD8: return ImGuiKey_Keypad8;
    case VK_NUMPAD9: return ImGuiKey_Keypad9;
    case VK_DECIMAL: return ImGuiKey_KeypadDecimal;
    case VK_DIVIDE: return ImGuiKey_KeypadDivide;
    case VK_MULTIPLY: return ImGuiKey_KeypadMultiply;
    case VK_SUBTRACT: return ImGuiKey_KeypadSubtract;
    case VK_ADD: return ImGuiKey_KeypadAdd;
    case VK_LSHIFT: return ImGuiKey_LeftShift;
    case VK_LCONTROL: return ImGuiKey_LeftCtrl;
    case VK_LMENU: return ImGuiKey_LeftAlt;
    case VK_LWIN: return ImGuiKey_LeftSuper;
    case VK_RSHIFT: return ImGuiKey_RightShift;
    case VK_RCONTROL: return ImGuiKey_RightCtrl;
    case VK_RMENU: return ImGuiKey_RightAlt;
    case VK_RWIN: return ImGuiKey_RightSuper;
    case VK_APPS: return ImGuiKey_Menu;
    case '0': return ImGuiKey_0;
    case '1': return ImGuiKey_1;
    case '2': return ImGuiKey_2;
    case '3': return ImGuiKey_3;
    case '4': return ImGuiKey_4;
    case '5': return ImGuiKey_5;
    case '6': return ImGuiKey_6;
    case '7': return ImGuiKey_7;
    case '8': return ImGuiKey_8;
    case '9': return ImGuiKey_9;
    case 'A': return ImGuiKey_A;
    case 'B': return ImGuiKey_B;
    case 'C': return ImGuiKey_C;
    case 'D': return ImGuiKey_D;
    case 'E': return ImGuiKey_E;
    case 'F': return ImGuiKey_F;
    case 'G': return ImGuiKey_G;
    case 'H': return ImGuiKey_H;
    case 'I': return ImGuiKey_I;
    case 'J': return ImGuiKey_J;
    case 'K': return ImGuiKey_K;
    case 'L': return ImGuiKey_L;
    case 'M': return ImGuiKey_M;
    case 'N': return ImGuiKey_N;
    case 'O': return ImGuiKey_O;
    case 'P': return ImGuiKey_P;
    case 'Q': return ImGuiKey_Q;
    case 'R': return ImGuiKey_R;
    case 'S': return ImGuiKey_S;
    case 'T': return ImGuiKey_T;
    case 'U': return ImGuiKey_U;
    case 'V': return ImGuiKey_V;
    case 'W': return ImGuiKey_W;
    case 'X': return ImGuiKey_X;
    case 'Y': return ImGuiKey_Y;
    case 'Z': return ImGuiKey_Z;
    case VK_F1: return ImGuiKey_F1;
    case VK_F2: return ImGuiKey_F2;
    case VK_F3: return ImGuiKey_F3;
    case VK_F4: return ImGuiKey_F4;
    case VK_F5: return ImGuiKey_F5;
    case VK_F6: return ImGuiKey_F6;
    case VK_F7: return ImGuiKey_F7;
    case VK_F8: return ImGuiKey_F8;
    case VK_F9: return ImGuiKey_F9;
    case VK_F10: return ImGuiKey_F10;
    case VK_F11: return ImGuiKey_F11;
    case VK_F12: return ImGuiKey_F12;
    case VK_F13: return ImGuiKey_F13;
    case VK_F14: return ImGuiKey_F14;
    case VK_F15: return ImGuiKey_F15;
    case VK_F16: return ImGuiKey_F16;
    case VK_F17: return ImGuiKey_F17;
    case VK_F18: return ImGuiKey_F18;
    case VK_F19: return ImGuiKey_F19;
    case VK_F20: return ImGuiKey_F20;
    case VK_F21: return ImGuiKey_F21;
    case VK_F22: return ImGuiKey_F22;
    case VK_F23: return ImGuiKey_F23;
    case VK_F24: return ImGuiKey_F24;
    case VK_BROWSER_BACK: return ImGuiKey_AppBack;
    case VK_BROWSER_FORWARD: return ImGuiKey_AppForward;
    default: return ImGuiKey_None;
    }
}

InputManager *instance = nullptr;

HOOK_TYPE(
    CanncelMouseEventHook,
    MouseDevice,
    sapphire::hook::HookPriority::Normal,
    MouseDevice::feed,
    void,
    char  actionButtonId,
    int   buttonData,
    short x,
    short y,
    short dx,
    short dy,
    bool  forceMotionlessPointer
) {
    if (GuiOverlay::sShowPannel || ImGui::GetIO().WantCaptureMouse)
        return;
    if (instance->mDisableGameMouseMoveInput)
        dx = dy = 0;
    if (instance->mDisableGameMouseWheelInput && actionButtonId == MouseAction::ActionType::ActionWheel)
        return;
    this->origin(actionButtonId, buttonData, x, y, dx, dy, forceMotionlessPointer);
}

void InputManager::init() {
    if (!this->mCoreWindow) return;
    try {
        this->mPointerPressedRevoker = this->mCoreWindow.PointerPressed(
            winrt::auto_revoke,
            [this](const CoreWindow &sender, const PointerEventArgs &args) {
                this->onPointerPressed(sender, args);
            }
        );
        this->mPointerReleasedRevoker = this->mCoreWindow.PointerReleased(
            winrt::auto_revoke,
            [this](const CoreWindow &sender, const PointerEventArgs &args) {
                this->onPointerReleased(sender, args);
            }
        );
        this->mPointerWheelRevoker = this->mCoreWindow.PointerWheelChanged(
            winrt::auto_revoke,
            [this](const CoreWindow &sender, const PointerEventArgs &args) {
                this->onPointerWheelChanged(sender, args);
            }
        );
    } catch (const winrt::hresult_error &ex) {
        Logger::Error("订阅 CoreWindowEvents 失败: {}", util::wstringToString(ex.message().c_str()));
        return;
    }
    this->mCoreDispatcher = this->mCoreWindow.Dispatcher();
    try {
        this->mAcceleratorRevoker = this->mCoreDispatcher.AcceleratorKeyActivated(
            winrt::auto_revoke,
            [this](const CoreDispatcher &sender, const AcceleratorKeyEventArgs &args) {
                this->onAcceleratorKeyActivated(sender, args);
            }
        );
    } catch (const winrt::hresult_error &ex) {
        Logger::Error("订阅 AcceleratorKeyActivated 失败: {}", util::wstringToString(ex.message().c_str()));
        return;
    }
    instance = this;
    CanncelMouseEventHook::hook();
}

InputManager::~InputManager() {
    CanncelMouseEventHook::unhook();
    this->mCoreDispatcher = nullptr;
    this->mCoreWindow = nullptr;
}

void InputManager::onAcceleratorKeyActivated(const CoreDispatcher &sender, const AcceleratorKeyEventArgs &args) {
    winrt::Windows::System::VirtualKey vk = args.VirtualKey();
    CoreAcceleratorKeyEventType        eventType = args.EventType();
    CorePhysicalKeyStatus              keyStatus = args.KeyStatus();

    const bool extendedkey = keyStatus.IsExtendedKey;
    const bool isPressed = eventType == CoreAcceleratorKeyEventType::KeyDown
                        || eventType == CoreAcceleratorKeyEventType::SystemKeyDown;
    ImGuiIO &io = ImGui::GetIO();
    switch (vk) {
    case winrt::Windows::System::VirtualKey::Menu:
        io.AddKeyEvent(extendedkey ? ImGuiKey::ImGuiKey_LeftAlt : ImGuiKey::ImGuiKey_RightAlt, isPressed);
        io.AddKeyEvent(ImGuiKey::ImGuiMod_Alt, isPressed);
        break;
    case winrt::Windows::System::VirtualKey::Control:
        io.AddKeyEvent(extendedkey ? ImGuiKey::ImGuiKey_LeftCtrl : ImGuiKey::ImGuiKey_RightCtrl, isPressed);
        io.AddKeyEvent(ImGuiKey::ImGuiMod_Ctrl, isPressed);
        break;
    case winrt::Windows::System::VirtualKey::Shift:
        io.AddKeyEvent(extendedkey ? ImGuiKey::ImGuiKey_LeftShift : ImGuiKey::ImGuiKey_RightShift, isPressed);
        io.AddKeyEvent(ImGuiKey::ImGuiMod_Shift, isPressed);
        break;
    case winrt::Windows::System::VirtualKey::Delete:
        io.AddKeyEvent(extendedkey ? ImGuiKey::ImGuiKey_Delete : ImGuiKey::ImGuiKey_KeypadDecimal, isPressed);
        break;
    default:
        if (ImGuiKey imguiKey = KeyEventToImGuiKey(static_cast<WPARAM>(vk)); imguiKey != ImGuiKey_None) {
            io.AddKeyEvent(imguiKey, isPressed);
        }
        break;
    }

    if (isPressed && io.WantTextInput) {
        BYTE keyboardState[256] = {0};
        GetKeyboardState(keyboardState);
        WCHAR charBuf[2] = {0};
        if (ToUnicode(static_cast<UINT>(vk), keyStatus.ScanCode, keyboardState, charBuf, 1, 0) > 0) {
            io.AddInputCharacterUTF16(charBuf[0]);
        }
    }

    if (io.WantCaptureKeyboard || mDisableGameKeyBoardInput) {
        args.Handled(true);
    }
}

void InputManager::onPointerPressed(const CoreWindow &sender, const PointerEventArgs &args) {
    if (ClientInstance::primaryClientInstance) {
        if (ClientInstance::primaryClientInstance->getMouseGrabbed())
            return;
    } else {
        Logger::Warn("primaryClientInstance not found");
    }
    ImGuiIO                      &io = ImGui::GetIO();
    Input::PointerPoint           point = args.CurrentPoint();
    Input::PointerPointProperties props = point.Properties();

    Input::PointerUpdateKind updateKind = props.PointerUpdateKind();
    int                      button = -1;
    switch (updateKind) {
    case Input::PointerUpdateKind::LeftButtonPressed: button = 0; break;
    case Input::PointerUpdateKind::RightButtonPressed: button = 1; break;
    case Input::PointerUpdateKind::MiddleButtonPressed: button = 2; break;
    case Input::PointerUpdateKind::XButton1Pressed: button = 3; break;
    case Input::PointerUpdateKind::XButton2Pressed: button = 4; break;
    default: break;
    }

    if (button != -1) {
        io.AddMouseButtonEvent(button, true);
    }
    if (GuiOverlay::sShowPannel || mDisableGamePointerInput) {
        args.Handled(true);
    }
}

void InputManager::onPointerReleased(const CoreWindow &sender, const PointerEventArgs &args) {
    ImGuiIO                      &io = ImGui::GetIO();
    Input::PointerPoint           point = args.CurrentPoint();
    Input::PointerPointProperties props = point.Properties();

    Input::PointerUpdateKind updateKind = props.PointerUpdateKind();
    int                      button = -1;
    switch (updateKind) {
    case Input::PointerUpdateKind::LeftButtonReleased: button = 0; break;
    case Input::PointerUpdateKind::RightButtonReleased: button = 1; break;
    case Input::PointerUpdateKind::MiddleButtonReleased: button = 2; break;
    case Input::PointerUpdateKind::XButton1Released: button = 3; break;
    case Input::PointerUpdateKind::XButton2Released: button = 4; break;
    default: break;
    }
    if (button != -1) {
        io.AddMouseButtonEvent(button, false);
    }
    if (GuiOverlay::sShowPannel || mDisableGamePointerInput) {
        args.Handled(true);
    }
}

void InputManager::onPointerWheelChanged(const CoreWindow &sender, const PointerEventArgs &args) {
    if (ClientInstance::primaryClientInstance) {
        if (!mDisableGameMouseWheelInput && ClientInstance::primaryClientInstance->getMouseGrabbed()) {
            return;
        }
    } else {
        Logger::Warn("primaryClientInstance not found");
    }

    using namespace Input;
    ImGuiIO               &io = ImGui::GetIO();
    PointerPoint           point = args.CurrentPoint();
    PointerPointProperties props = point.Properties();

    int32_t wheelDelta = props.MouseWheelDelta();
    if (wheelDelta != 0) {
        float wheelAmount = (float)wheelDelta / (float)WHEEL_DELTA;
        float wheel_y = 0.0f;
        float wheel_x = 0.0f;
        (props.IsHorizontalMouseWheel() ? wheel_x : wheel_y) = wheelAmount;
        io.AddMouseWheelEvent(wheel_x, wheel_y);
    }
    if (GuiOverlay::sShowPannel || mDisableGameMouseWheelInput) {
        args.Handled(true);
    }
}
