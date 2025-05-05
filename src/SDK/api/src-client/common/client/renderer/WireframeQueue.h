#pragma once

#include <unordered_map>
#include "Wireframe.h"

class WireframeQueue
{
public:
    std::unordered_map<BlockPos, Wireframe> mQueue;
};
