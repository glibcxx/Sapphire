#pragma once

#include <vector>
#include "SDK/api/src-deps/Core/Memory/LinearAllocator.h"

struct ViewRenderObject;

class FrameRenderObject {
public:
    std::vector<ViewRenderObject, LinearAllocator<ViewRenderObject>> mViews; // off+0
};