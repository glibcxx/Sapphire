#pragma once

#include "SDK/api/src/common/world/Container.h"

class PlayerInventory {
public:
    // size: 8
    struct SlotData {
        ContainerID mContainerId = ContainerID::CONTAINER_ID_NONE; // off+0
        int         mSlot = -1;                                    // off+4
    };
};