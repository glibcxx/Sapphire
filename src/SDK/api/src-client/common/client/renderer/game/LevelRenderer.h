#pragma once

#include "SDK/api/src-client/common/client/gui/screen/ScreenContext.h"
#include "LevelRendererPlayer.h"

class LevelRenderer {
public:
    SDK_API void renderLevel(ScreenContext &screenCtx, const class FrameRenderObject &frameRenderObj);

    SDK_API void preRenderUpdate(ScreenContext &screenCtx, class LevelRenderPreRenderUpdateParameters &params);

    LevelRendererPlayer &getLevelRendererPlayer() {
#if MC_VERSION == v1_21_2
        return memory::getField<LevelRendererPlayer &>(this, 776);
#elif MC_VERSION == v1_21_50
        return memory::getField<LevelRendererPlayer &>(this, 792);
#elif MC_VERSION == v1_21_60
        return memory::getField<LevelRendererPlayer &>(this, 808);
#endif
    }
};