#pragma once

#include "SDK/api/src/common/world/level/IConstBlockSource.h"
#include "SDK/api/src/common/world/level/block/actor/BlockActor.h"

struct BlockActorRenderData {
    const IConstBlockSource *mRegion;
    BlockActor              *mBlockActor;
    Vec3                    *mPos;
    // ...
};