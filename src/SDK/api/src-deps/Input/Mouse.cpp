#include "Mouse.h"

MouseDevice &Mouse::_instance = memory::getField<MouseDevice>(
    reinterpret_cast<void *>(
        sapphire::loadStatic<
            "\x48\x63\x05\x00\x00\x00\x00\x4C\x8B\x1D"_sig,
            &Mouse::_instance,
            sapphire::deRefMov>()
    ),
    0
);