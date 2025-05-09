#pragma once

#include "SDK/api/src/common/world/phys/Vec3.h"

class LevelRendererPlayer {
public:
    Vec3 &getCameraPosition() {
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
};