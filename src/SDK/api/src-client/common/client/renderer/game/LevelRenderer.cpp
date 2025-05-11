#include "LevelRenderer.h"

#include "SDK/api/sapphire/hook/Hook.h"

void LevelRenderer::renderLevel(ScreenContext &screenCtx, const FrameRenderObject &frameRenderObj) {
    using Hook = core::ApiLoader<
#if MC_VERSION == v1_21_2
        "\xE8\x00\x00\x00\x00\x45\x32\xE4\x48\x8B\x07"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\xE8\x00\x00\x00\x00\xC6\x44\x24\x00\x00\x48\x8B\x86\x00\x00\x00\x00\x48\x85\xC0"_sig,
#endif
        &LevelRenderer::renderLevel,
        [](uintptr_t addr) -> uintptr_t {
            auto a = memory::deRef(addr, memory::AsmOperation::CALL);
            hook::HookManager::getInstance().resitryApi(util::ApiUniqueId::make<&LevelRenderer::renderLevel>(), a);
            return a;
        }>;
    (this->*Hook::origin)(screenCtx, frameRenderObj);
}

void LevelRenderer::preRenderUpdate(ScreenContext &screenCtx, LevelRenderPreRenderUpdateParameters &params) {
    using Hook = core::ApiLoader<
#if MC_VERSION == v1_21_2
        "\xE8\x00\x00\x00\x00\xF3\x41\x0F\x10\x4D\x00\x48\x8B\xCE"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\xE8\x00\x00\x00\x00\xF3\x41\x0F\x10\x4C\x24\x00\x48\x8B\xCE"_sig,
#endif
        &LevelRenderer::preRenderUpdate,
        [](uintptr_t addr) -> uintptr_t {
            auto a = memory::deRef(addr, memory::AsmOperation::CALL);
            hook::HookManager::getInstance().resitryApi(util::ApiUniqueId::make<&LevelRenderer::preRenderUpdate>(), a);
            Logger::InfoBox(L"Info");
            return a;
        }>;
    (this->*Hook::origin)(screenCtx, params);
}
