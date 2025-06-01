#include "LevelRendererPlayer.h"

void LevelRendererPlayer::setupCamera(mce::Camera &camera, const float a) {
    using Hook = core::ApiLoader<
        "\xE8\x00\x00\x00\x00\xF3\x41\x0F\x10\x00\x08\x00\x8B\x00\x00\x03"_sig,
        &LevelRendererPlayer::setupCamera,
        [](uintptr_t addr) {
            return memory::deRef(addr, memory::AsmOperation::CALL);
        }>;
    (this->*Hook::origin)(camera, a);
}
