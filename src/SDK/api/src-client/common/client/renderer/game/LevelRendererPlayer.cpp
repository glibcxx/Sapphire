#include "LevelRendererPlayer.h"

void LevelRendererPlayer::setupCamera(mce::Camera &camera, const float a) {
    using Hook = sapphire::ApiLoader<
        "\xE8\x00\x00\x00\x00\xF3\x41\x0F\x10\x00\x08\x00\x8B\x00\x00\x03"_sig,
        &LevelRendererPlayer::setupCamera,
        [](uintptr_t addr) {
            return memory::deRef(addr, memory::AsmOperation::CALL);
        }>;
    (this->*Hook::origin)(camera, a);
}

void LevelRendererPlayer::queueRenderEntities(LevelRenderPreRenderUpdateParameters &levelRenderPreRenderUpdateParameters) {
    /*
        just search "queueRenderEntities"
    */
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        "\x40\x53\x57\x41\x57\x48\x83\xEC\x00\x48\x8B\xF9"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x40\x53\x55\x41\x55\x48\x83\xEC\x00\x48\x8B\x05"_sig,
#endif
        &LevelRendererPlayer::queueRenderEntities>;
    (this->*Hook::origin)(levelRenderPreRenderUpdateParameters);
}

void LevelRendererPlayer::callRenderCracks(BaseActorRenderContext &renderContext, const ViewRenderObject &renderObj) {
        using Hook = sapphire::ApiLoader<
        "\x48\x8B\xC4\x48\x89\x58\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x4C\x8B\xFA"_sig,
        &LevelRendererPlayer::callRenderCracks>;
    (this->*Hook::origin)(renderContext, renderObj);
}
