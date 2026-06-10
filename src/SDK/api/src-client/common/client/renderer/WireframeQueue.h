#pragma once

#include "pch.h" // IWYU pragma: keep

#include "Wireframe.h"

class WireframeQueue
{
public:
    std::unordered_map<BlockPos, Wireframe> mQueue;
};
