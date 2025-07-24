#pragma once

#include "SDK/api/src/common/world/actor/player/Player.h"
#include "SDK/api/src/common/entity/components/MoveInputComponent.h"

class LocalPlayer : public Player {
public:
    SDK_API void localPlayerTurn(const Vec2 &deltaRot);
    MARK_HOOKABLE(&LocalPlayer::localPlayerTurn)

    MoveInputComponent *getMoveInputComponent() {
        return tryGetComponent<MoveInputComponent>();
    }

    const MoveInputComponent *getMoveInputComponent() const {
        return tryGetComponent<MoveInputComponent>();
    }
};