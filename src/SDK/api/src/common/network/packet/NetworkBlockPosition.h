#pragma once

#include "pch.h" // IWYU pragma: keep

#include "SDK/api/src/common/world/level/BlockPos.h"

// size: 12
class NetworkBlockPosition : public BlockPos {
public:
    using BlockPos::BlockPos;
};