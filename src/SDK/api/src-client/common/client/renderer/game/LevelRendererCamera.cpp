#include "LevelRendererCamera.h"

void LevelRendererCamera::preRenderUpdate(
    ScreenContext                        &screenContext,
    LevelRenderPreRenderUpdateParameters &levelRenderPreRenderUpdateParameters
) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xF3\x0F\x10\x00\x00\x66\x0F\x6E\x8B"_sig,
        &LevelRendererCamera::preRenderUpdate>;
    (this->*Hook::origin)(screenContext, levelRenderPreRenderUpdateParameters);
}

void LevelRendererCamera::render(
    BaseActorRenderContext       &baseEntityRenderContext,
    const ViewRenderObject       &renderObj,
    IClientInstance              &ci,
    LevelRendererCommandListInit &levelRendererCommandListInit
) {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        "\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4D\x8B\xE9\x4C\x89\x4D\x00\x49\x8B\xF0"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4C\x89\x4D\x00\x4D\x8B\xE8\x48\x89\x54\x24"_sig,
#endif
        &LevelRendererCamera::render>;
    (this->*Hook::origin)(baseEntityRenderContext, renderObj, ci, levelRendererCommandListInit);
}
