#include "LevelRendererCamera.h"

void LevelRendererCamera::preRenderUpdate(
    ScreenContext                        &screenContext,
    LevelRenderPreRenderUpdateParameters &levelRenderPreRenderUpdateParameters
) {
    using Hook = core::ApiLoader<
        "\xE8\x00\x00\x00\x00\xF3\x0F\x10\x00\x00\x66\x0F\x6E\x8B"_sig,
        &LevelRendererCamera::preRenderUpdate,
        [](uintptr_t addr) {
            return memory::deRef(addr, memory::AsmOperation::CALL);
        }>;
    (this->*Hook::origin)(screenContext, levelRenderPreRenderUpdateParameters);
}
