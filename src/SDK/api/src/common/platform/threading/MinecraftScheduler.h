#pragma once

#include "SDK/api/src-deps/Core/Threading/Scheduler.h"

class MinecraftScheduler {
public:
    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\xF2\x0F\x10\x05\x00\x00\x00\x00\xF2\x0F\x11\x40\x00\xE8")
    SDK_API static Scheduler &client();
};