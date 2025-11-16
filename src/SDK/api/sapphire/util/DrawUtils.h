#pragma once

#include <mutex>
#include "SDK/api/src/common/world/phys/AABB.h"
#include "SDK/api/src-client/common/client/renderer/Tessellator.h"
#include "SDK/api/src-client/common/client/renderer/MaterialPtr.h"
#include "SDK/api/src-client/common/client/renderer/game/LevelRenderer.h"

class DrawUtils {
    friend class RenderLevelMainFuncHook;

    Tessellator   *mTess = nullptr;
    ScreenContext *mScreenCtx = nullptr;
    LevelRenderer *mLevelRenderer = nullptr;

    mutable std::mutex mMutex;

    DrawUtils(Tessellator *tess = nullptr);

    ~DrawUtils();

    static mce::MaterialPtr sDrawMat;

public:
    SPHR_API void drawLine(const Vec3 &from, const Vec3 &to, const mce::Color &color = mce::Colors::RED) const;

    SPHR_API void drawBox(
        const AABB       &aabb,
        const mce::Color &color = mce::Colors::RED
    ) const;

    SPHR_API void drawQuard(
        const Vec3 &a,
        const Vec3 &b,
        const Vec3 &c,
        const Vec3 &d,
        const mce::Color &color = mce::Colors::RED
    ) const;

    void drawBox(
        const Vec3       &min,
        const Vec3       &max,
        const mce::Color &color = mce::Colors::RED
    ) const {
        drawBox({min, max}, color);
    }

    SPHR_API void flush();

    SPHR_API static DrawUtils &getInstance();

    Tessellator *getTessellator() { return mTess; }
};