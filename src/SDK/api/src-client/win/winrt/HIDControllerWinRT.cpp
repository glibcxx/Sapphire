#include "HIDControllerWinRT.h"

HIDControllerWinRT *HIDControllerWinRT::ctor(
    AppPlatformWinRTDispatcher &dispatcher, winrt::Windows::UI::Core::CoreWindow coreWindow
) {
    // ??0HIDControllerWinRT@@QEAA@AEAVAppPlatformWinRTDispatcher@@UCoreWindow@Core@UI@Windows@winrt@@@Z
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        "\xE8\x00\x00\x00\x00\x90\x48\x83\x3F\x00\x74\x00\x48\x8B\xCF\xE8\x00\x00\x00\x00\x48\x8B\xC3\x48\x83\xC4"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\xE8\x00\x00\x00\x00\x90\x48\x83\x3F\x00\x74\x00\x48\x8B\xCF\xE8\x00\x00\x00\x00\x48\x8B\xC3\x48\x8B\x4C\x24"_sig,
#endif
        &HIDControllerWinRT::ctor,
        sapphire::deRefCall>;
    return (this->*Hook::origin)(dispatcher, std::move(coreWindow));
}