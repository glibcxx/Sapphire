#include "GameControllerHandler_winrt.h"

GameControllerHandler_winrt::WGIGamepad *GameControllerHandler_winrt::WGIGamepad::ctor(
    winrt::Windows::Gaming::Input::Gamepad gamepad
) {
    using Bind = sapphire::bind::Fn<
        [](uintptr_t addr) { return memory::deRef(addr + 15, memory::AsmOperation::CALL); }
            | "\xFF\x15\x00\x00\x00\x00\x48\x8D\x54\x24\x00\x48\x8D\x00\x00\xE8\x00\x00\x00\x00\xEB\x00\x48"_sig,
        &WGIGamepad::ctor>;
    return (this->*Bind::origin)(std::move(gamepad));
}
