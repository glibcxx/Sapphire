#pragma once

#include <set>
#include <winrt/windows.ui.core.h>
#include <winrt/windows.ui.text.core.h>
#include <winrt/windows.devices.input.h>

#include "SDK/core/ApiManager.h"

class AppPlatformWinRTDispatcher;
class GameControllerHandler;

// size: 8848
class HIDControllerWinRT {
public:
    AppPlatformWinRTDispatcher                            &mDispatcher; // off+0
    winrt::agile_ref<winrt::Windows::UI::Core::CoreWindow> mCoreWindow; // off+8

    bool mUnk16; // off+16
    bool mUnk17; // off+17
    bool mUnk18; // off+18
    bool mUnk19; // off+19
    bool mUnk20; // off+20
    bool mUnk21; // off+21
    bool mUnk22; // off+22
    bool mUnk23; // off+23

    bool mMouseLeftPressed;   // off+24
    bool mMouseRightPressed;  // off+25
    bool mMouseMiddlePressed; // off+26
    bool mMouseX1Pressed;     // off+27
    bool mMouseX2Pressed;     // off+28

    uint32_t                                            mUnk32;                    // off+32
    uint32_t                                            mUnk36;                    // off+36
    uint8_t                                             mUnk40[8];                 // off+40
    std::map<uint32_t, uint8_t>                         mMouseButtonMap;           // off+48
    std::string                                         mInputTextBuffer;          // off+64
    std::vector<std::unique_ptr<GameControllerHandler>> mGameControllerHandler;    // off+96
    std::mutex                                          mUnk120;                   // off+120
    uint64_t                                            mUnk200;                   // off+200
    uint8_t                                             mUnk208;                   // off+208
    bool                                                mLayoutChanged;            // off+208
    uint64_t                                            mUnk216[4];                // off+216
    winrt::Windows::Devices::Input::MouseCapabilities   mMouseCapabilities;        // off+248
    uint16_t                                            mPointerPos_x;             // off+256
    uint16_t                                            mPointerPos_y;             // off+258
    uint8_t                                             mTextInputActive;          // off+260
    bool                                                mVirtualKeyStatusMap[256]; // off+261, see winrt::Windows::System::VirtualKey

    winrt::event_token mOnCharacterReceived;       // off+520
    winrt::event_token mOnKeyDown;                 // off+528
    winrt::event_token mOnKeyUp;                   // off+536
    winrt::event_token mOnMouseMoved;              // off+544
    winrt::event_token mOnAcceleratorKeyActivated; // off+552

    uint8_t                                             mUnk560;          // off+560
    std::string                                         mUnk568[256];     // off+568
    winrt::Windows::UI::Text::Core::CoreTextEditContext mTextEditContext; // off+8760
    winrt::Windows::UI::Text::Core::CoreTextRange       mSelectionRange;  // off+8768

    winrt::event_token mOnTextInputCompositionCompleted; // off+8776, 输入法组词结束
    winrt::event_token mOnTextInputCompositionStarted;   // off+8784, 输入法组词开始
    winrt::event_token mOnTextInputFocusRemoved;         // off+8792, 输入控件失去焦点
    winrt::event_token mOnTextInputFormatUpdating;       // off+8800, 输入带格式文本的格式变化
    winrt::event_token mOnTextInputLayoutRequested;      // off+8808, 请求输入控件布局信息
    winrt::event_token mOnTextInputSelectionRequested;   // off+8816, 请求输入选区
    winrt::event_token mOnTextInputSelectionUpdating;    // off+8824, 更新输入选区
    winrt::event_token mOnTextInputTextRequested;        // off+8832, 请求输入控件的文本
    winrt::event_token mOnTextInputTextUpdating;         // off+8840, 更新输入文本

    SDK_API HIDControllerWinRT *ctor(AppPlatformWinRTDispatcher &dispatcher, winrt::Windows::UI::Core::CoreWindow coreWindow);
    MARK_HOOKABLE(&HIDControllerWinRT::ctor)
};
static_assert(sizeof(HIDControllerWinRT) == 8848);