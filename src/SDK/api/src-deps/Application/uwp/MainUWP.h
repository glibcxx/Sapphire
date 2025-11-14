#pragma once

#include <winrt/windows.applicationmodel.core.h>
#include "SDK/api/src-deps/Core/Platform/ActivationArguments.h"
#include "SDK/core/ApiManager.h"

namespace Bedrock {

    class Platform_UWP;

    // size: 288
    class MainWindowUWP : public winrt::implements<MainWindowUWP, winrt::Windows::ApplicationModel::Core::IFrameworkView> {
    public:
        uint64_t                      mUnk24;               // off+24
        bool                          mWantToQuit;          // off+32
        bool                          mWindowVisible;       // off+33
        uint8_t                       mUnk34;               // off+34
        Bedrock::ActivationArguments  mActivationArguments; // off+40
        std::unique_ptr<Platform_UWP> mPlatform;            // off+280

        SDK_API void Initialize(const winrt::Windows::ApplicationModel::Core::CoreApplicationView &applicationView);

        void Load(const winrt::hstring &entryPoint);

        SDK_API void Run();

        SDK_API void SetWindow(const winrt::Windows::UI::Core::CoreWindow &window);

        void Uninitialize();
    };

    // size: 24
    class MainUWP : public winrt::implements<MainUWP, winrt::Windows::ApplicationModel::Core::IFrameworkViewSource> {
    public:
        winrt::Windows::ApplicationModel::Core::IFrameworkView CreateView();
    };

} // namespace Bedrock