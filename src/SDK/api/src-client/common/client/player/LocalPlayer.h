#pragma once

#include "SDK/api/src/common/world/actor/player/Player.h"
#include "SDK/api/src/common/entity/components/MoveInputComponent.h"

class LocalPlayer : public Player {
public:
    SAPPHIRE_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x90\x48\x85\xF6\x74\x00\x48\x8B\xCE\xE8\x00\x00\x00\x00\x0F\x28\xCE")
    SAPPHIRE_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x49\x8B\x06\x0F\x28\xCE")
    SDK_API void localPlayerTurn(const Vec2 &deltaRot);

    MoveInputComponent *getMoveInputComponent() {
        return tryGetComponent<MoveInputComponent>();
    }

    const MoveInputComponent *getMoveInputComponent() const {
        return tryGetComponent<MoveInputComponent>();
    }
};