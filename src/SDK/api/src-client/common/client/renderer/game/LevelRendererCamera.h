#pragma once

#include "SDK/api/src/common/world/actor/player/PlayerListener.h"
#include "SDK/api/src/common/world/level/LevelListener.h"
#include "SDK/api/src-client/common/client/gui/screen/ScreenContext.h"

class LevelRenderPreRenderUpdateParameters;

class LevelRendererCamera : public PlayerListener,
                            public LevelListener {
public:
    /*virtual*/ Vec3 &getCameraPos() {
        /*
            search "rebuildClouds task" and find
            `v7.m128_f32[0] = (float)((float)(v8 + a3) * 0.02) + *(float *)(a1 + {here it is});`
            at the top
        */

#if MC_VERSION == v1_21_2
        return memory::getField<Vec3>(this, 1544);
#elif MC_VERSION == v1_21_50
        return memory::getField<Vec3>(this, 1560);
#elif MC_VERSION == v1_21_60
        return memory::getField<Vec3>(this, 1552);
#endif
    }

    /*virtual*/ void preRenderUpdate(
        ScreenContext                        &screenContext,
        LevelRenderPreRenderUpdateParameters &levelRenderPreRenderUpdateParameters
    ) /*override*/;
};