#pragma once

#include "SDK/api/src/common/world/level/BlockSource.h"
#include "SDK/api/src/common/world/level/block/actor/BlockActor.h"
#include "src/SDK/api/src-client/common/client/renderer/MaterialPtr.h"

struct BlockActorRenderData {
    BlockSource            *renderSource;
    BlockActor             *entity;
    const class Block      *block;
    const Vec3             *position;
    const mce::MaterialPtr *forcedMat;
    // ...
};