#pragma once

#include "SDK/api/src-client/common/client/gui/screen/ScreenContext.h"

class InGamePlayScreen {
public:
    /*virtual*/ void prepareFrame(ScreenContext &screenContext) /*override*/;
    /*virtual*/ void render(ScreenContext &screenContext, const FrameRenderObject &renderObj) /*override*/;

    static void *const *__vftable;
};