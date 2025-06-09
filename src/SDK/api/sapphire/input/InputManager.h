#pragma once

#include <Windows.h>
#include <winrt/Windows.UI.Core.h>

class InputManager {
public:
    using CoreWindow = winrt::Windows::UI::Core::CoreWindow;

    InputManager(CoreWindow &coreWindow) :
        mCoreWindow(coreWindow) {
        this->init();
    }

    ~InputManager();

    void disableGameKeyBoardInput(bool disable) {
        this->mDisableGameKeyBoardInput = disable;
    }

    void disableGamePointerInput(bool disable) {
        this->mDisableGamePointerInput = disable;
    }

    void disableGameMouseWheelInput(bool disable) {
        this->mDisableGameMouseWheelInput = disable;
    }

    void disableGameMouseMoveInput(bool disable) {
        this->mDisableGameMouseMoveInput = disable;
    }

private:
    friend class CanncelMouseEventHook;

    void init();

    using CoreDispatcher = winrt::Windows::UI::Core::CoreDispatcher;
    using PointerEventArgs = winrt::Windows::UI::Core::PointerEventArgs;
    using AcceleratorKeyEventArgs = winrt::Windows::UI::Core::AcceleratorKeyEventArgs;

    void onAcceleratorKeyActivated(const CoreDispatcher &sender, const AcceleratorKeyEventArgs &args);

    void onPointerPressed(const CoreWindow &sender, const PointerEventArgs &args);

    void onPointerReleased(const CoreWindow &sender, const PointerEventArgs &args);

    void onPointerWheelChanged(const CoreWindow &sender, const PointerEventArgs &args);

    CoreWindow     mCoreWindow{nullptr};
    CoreDispatcher mCoreDispatcher{nullptr};

    CoreWindow::PointerPressed_revoker              mPointerPressedRevoker;
    CoreWindow::PointerReleased_revoker             mPointerReleasedRevoker;
    CoreWindow::PointerWheelChanged_revoker         mPointerWheelRevoker;
    CoreDispatcher::AcceleratorKeyActivated_revoker mAcceleratorRevoker;

    bool mDisableGameKeyBoardInput = false;
    bool mDisableGamePointerInput = false;
    bool mDisableGameMouseWheelInput = false;
    bool mDisableGameMouseMoveInput = false;
};