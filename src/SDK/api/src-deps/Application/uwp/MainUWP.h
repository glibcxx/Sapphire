#pragma once

#include <winrt/windows.applicationmodel.core.h>
#include "SDK/api/src-deps/Core/Platform/ActivationArguments.h"
#include "SDK/core/SymbolResolver.h"

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

        SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x33\xC0\xEB\x00\x8B\x44\x24\x00\x48\x83\xC4\x00\x5B\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x45\x33\xC0")
        SDK_API void Initialize(const winrt::Windows::ApplicationModel::Core::CoreApplicationView &applicationView);

        void Load(const winrt::hstring &entryPoint);

        SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x33\xC0\xEB\x00\x8B\x44\x24\x00\x48\x83\xC4\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC")
        SDK_API void Run();

        SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x33\xC0\xEB\x00\x8B\x44\x24\x00\x48\x83\xC4\x00\x5B\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x40\x53")
        SDK_API void SetWindow(const winrt::Windows::UI::Core::CoreWindow &window);

        void Uninitialize();
    };

    // size: 24
    class MainUWP : public winrt::implements<MainUWP, winrt::Windows::ApplicationModel::Core::IFrameworkViewSource> {
    public:
        winrt::Windows::ApplicationModel::Core::IFrameworkView CreateView();
    };

} // namespace Bedrock