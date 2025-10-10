#pragma once

#include "BaseScreen.h"
#include "SDK/api/src-client/common/client/gui/screens/ScreenContext.h"

class InGamePlayScreen : public BaseScreen {
public:
    SDK_API /*virtual*/ void tick(int32_t nTick, int32_t maxTick) /*override*/;

    SDK_API /*virtual*/ void applyInput(float a) /*override*/;

    SDK_API /*virtual*/ void prepareFrame(ScreenContext &screenContext) /*override*/;

    SDK_API /*virtual*/ void render(ScreenContext &screenContext, const FrameRenderObject &renderObj) /*override*/;

    SDK_API void _pick(float a);

    static void *const *__vftable;
};