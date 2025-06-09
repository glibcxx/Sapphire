#pragma once

#include "SDK/api/src/common/world/phys/Vec2.h"

class LocalPlayer {
public:
    void localPlayerTurn(const Vec2 &deltaRot);
};