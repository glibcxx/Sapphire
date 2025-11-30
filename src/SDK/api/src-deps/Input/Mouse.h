#pragma once

#include "MouseDevice.h"

class Mouse {
public:
    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "mov", "\x48\x63\x05\x00\x00\x00\x00\x4C\x8B\x1D")
    SDK_API static MouseDevice *_instance;
};