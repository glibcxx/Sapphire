#include "AppPlatform_Winrt.h"

AppPlatform_Winrt *AppPlatform_Winrt::ctor(winrt::Windows::UI::Core::CoreWindow coreWindow) {
    using Hook = sapphire::ApiLoader<
        "\x4C\x8B\xDC\x49\x89\x5B\x00\x49\x89\x73\x00\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x41\x0F\x29\x73"_sig,
        &AppPlatform_Winrt::ctor>;
    return (this->*Hook::origin)(std::move(coreWindow));
}

void AppPlatform_Winrt::update() {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        "\xE8\x00\x00\x00\x00\x48\x8B\x73\x00\x0F\x1F\x40"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\xE8\x00\x00\x00\x00\x48\x8B\x7B\x00\x0F\x1F\x80"_sig,
#endif
        &AppPlatform_Winrt::update,
        sapphire::deRefCall>;
    (this->*Hook::origin)();
}