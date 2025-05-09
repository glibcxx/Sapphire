#pragma once

#include "SDK/api/src-client/common/client/renderer/Tessellator.h"
#include "SDK/api/src-client/common/client/renderer/MaterialPtr.h"
#include "SDK/api/src-client/common/client/renderer/game/LevelRenderer.h"

class DrawUtils {
    friend class GetScreenCtx;

    Tessellator   *mTess = nullptr;
    ScreenContext *mScreenCtx = nullptr;
    LevelRenderer *mLevelRenderer = nullptr;

    DrawUtils(Tessellator *tess = nullptr);

    ~DrawUtils();

    static mce::MaterialPtr sDrawMat;

public:
    SDK_API void drawLine(const Vec3 &from, const Vec3 &to, const mce::Color &color = mce::Colors::RED);

    SDK_API void flush();

    SDK_API static DrawUtils &getInstance();
};