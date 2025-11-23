#include "Mouse.h"

MouseDevice *Mouse::_instance = &sapphire::bind::data<
    sapphire::deRefMov | "\x48\x63\x05\x00\x00\x00\x00\x4C\x8B\x1D"_sig,
    &Mouse::_instance,
    MouseDevice>();
