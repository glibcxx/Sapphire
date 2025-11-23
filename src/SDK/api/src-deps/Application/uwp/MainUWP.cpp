#include "MainUWP.h"

void Bedrock::MainWindowUWP::Initialize(const winrt::Windows::ApplicationModel::Core::CoreApplicationView &applicationView) {
    using Bind = sapphire::bind::Fn<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x33\xC0\xEB\x00\x8B\x44\x24\x00\x48\x83\xC4\x00\x5B\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x45\x33\xC0"_sig,
        &Bedrock::MainWindowUWP::Initialize>;
    (this->*Bind::origin)(applicationView);
}

void Bedrock::MainWindowUWP::Run() {
    using Bind = sapphire::bind::Fn<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x33\xC0\xEB\x00\x8B\x44\x24\x00\x48\x83\xC4\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC"_sig,
        &Bedrock::MainWindowUWP::Run>;
    (this->*Bind::origin)();
}

void Bedrock::MainWindowUWP::SetWindow(const winrt::Windows::UI::Core::CoreWindow &window) {
    using Bind = sapphire::bind::Fn<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x33\xC0\xEB\x00\x8B\x44\x24\x00\x48\x83\xC4\x00\x5B\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x40\x53"_sig,
        &Bedrock::MainWindowUWP::SetWindow>;
    (this->*Bind::origin)(window);
}
