#include "LocalPlayer.h"

void LocalPlayer::localPlayerTurn(const Vec2 &deltaRot) {
    using Hook = core::ApiLoader<
#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\xE8\x00\x00\x00\x00\x49\x8B\x06\x0F\x28\xCE"_sig,
#endif
        &LocalPlayer::localPlayerTurn,
        core::deRefCall>;
    (this->*Hook::origin)(deltaRot);
}