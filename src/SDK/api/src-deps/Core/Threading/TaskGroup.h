#pragma once

#include "SDK/core/Core.h"

class TaskGroup {
public:
#if MC_VERSION == v1_21_2
    uint8_t fill[320];
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    uint8_t fill[336];
#endif
};