#pragma once

#include "SDK/api/src-client/common/client/gui/screen/ScreenContext.h"
#include "SDK/api/src/common/world/level/LevelListener.h"
#include "SDK/api/src/common/AppPlatformListener.h"
#include "LevelRendererPlayer.h"
#include "LevelRenderPreRenderUpdateParameters.h"

class LevelRenderer : public LevelListener, public AppPlatformListener {
public:
    SDK_API void renderLevel(ScreenContext &screenCtx, const FrameRenderObject &frameRenderObj);
    MARK_HOOKABLE(&LevelRenderer::renderLevel)

    SDK_API void preRenderUpdate(ScreenContext &screenCtx, LevelRenderPreRenderUpdateParameters &params);
    MARK_HOOKABLE(&LevelRenderer::preRenderUpdate)

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