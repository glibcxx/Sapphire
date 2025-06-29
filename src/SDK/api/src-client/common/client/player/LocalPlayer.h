#pragma once

#include "SDK/api/src/common/world/actor/player/Player.h"
#include "SDK/api/src/common/entity/components/MoveInputComponent.h"

class LocalPlayer : public Player {
public:
    void localPlayerTurn(const Vec2 &deltaRot);

    MoveInputComponent *getMoveInputComponent() {
        return tryGetComponent<MoveInputComponent>();
    }

    const MoveInputComponent *getMoveInputComponent() const {
        return tryGetComponent<MoveInputComponent>();
    }
};