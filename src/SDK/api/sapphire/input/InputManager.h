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

private:
    void init();

    using CoreDispatcher = winrt::Windows::UI::Core::CoreDispatcher;
    using PointerEventArgs = winrt::Windows::UI::Core::PointerEventArgs;
    using AcceleratorKeyEventArgs = winrt::Windows::UI::Core::AcceleratorKeyEventArgs;

    static void onAcceleratorKeyActivated(const CoreDispatcher &sender, const AcceleratorKeyEventArgs &args);

    static void onPointerPressed(const CoreWindow &sender, const PointerEventArgs &args);

    static void onPointerReleased(const CoreWindow &sender, const PointerEventArgs &args);

    static void onPointerWheelChanged(const CoreWindow &sender, const PointerEventArgs &args);

    CoreWindow     mCoreWindow{nullptr};
    CoreDispatcher mCoreDispatcher{nullptr};

    CoreWindow::PointerPressed_revoker              mPointerPressedRevoker;
    CoreWindow::PointerReleased_revoker             mPointerReleasedRevoker;
    CoreWindow::PointerWheelChanged_revoker         mPointerWheelRevoker;
    CoreDispatcher::AcceleratorKeyActivated_revoker mAcceleratorRevoker;
};