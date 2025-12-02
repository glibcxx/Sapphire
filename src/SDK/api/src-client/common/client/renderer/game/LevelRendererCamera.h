#pragma once

#include "SDK/api/src/common/world/actor/player/PlayerListener.h"
#include "SDK/api/src/common/world/level/LevelListener.h"
#include "SDK/api/src-client/common/client/gui/screens/ScreenContext.h"
#include "SDK/api/src-client/common/client/renderer/BaseActorRenderContext.h"
#include "LevelRenderPreRenderUpdateParameters.h"
#include "util/Memory.hpp"

struct LevelRendererCommandListInit;

class LevelRendererCamera {
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

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\xF3\x0F\x10\x00\x00\x66\x0F\x6E\x8B")
    SDK_API /*virtual*/ void preRenderUpdate(
        ScreenContext                        &screenContext,
        LevelRenderPreRenderUpdateParameters &levelRenderPreRenderUpdateParameters
    );

    SPHR_DECL_API("1.21.2", "\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4D\x8B\xE9\x4C\x89\x4D\x00\x49\x8B\xF0")
    SPHR_DECL_API("1.21.50,1.21.60", "\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4C\x89\x4D\x00\x4D\x8B\xE8\x48\x89\x54\x24")
    SDK_API /*virtual*/ void render(
        BaseActorRenderContext       &baseEntityRenderContext,
        const ViewRenderObject       &renderObj,
        IClientInstance              &ci,
        LevelRendererCommandListInit &levelRendererCommandListInit
    );
};