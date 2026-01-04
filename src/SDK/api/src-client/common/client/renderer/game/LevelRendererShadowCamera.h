#pragma once

#include "SDK/api/src-client/common/client/renderer/game/LevelRendererCamera.h"

class LevelRendererShadowCamera : public LevelRendererCamera {
public:
    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x48\x8B\x8F\x00\x00\x00\x00\x48\x8B\x13")
    SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x49\x8B\xAF\x00\x00\x00\x00\x49\x8B\x36")
    SDK_API void preRenderUpdate(ScreenContext &screenCtx, LevelRenderPreRenderUpdateParameters &params);
};