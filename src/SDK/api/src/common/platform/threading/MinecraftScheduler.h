#pragma once

#include "SDK/api/src-deps/Core/Threading/Scheduler.h"

class MinecraftScheduler {
public:
    SDK_API static Scheduler &client();
};