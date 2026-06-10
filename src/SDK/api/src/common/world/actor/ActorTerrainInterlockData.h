#pragma once

#include "pch.h" // IWYU pragma: keep

class ActorTerrainInterlockData {
public:
    enum class VisibilityState : uint8_t {
        InitialNotVisible = 0,
        Visible = 1,
        DelayedDestructionNotVisible = 2,
    };

    VisibilityState                       mRenderVisibilityState; // off+0
    std::chrono::system_clock::time_point mCreationTime;          // off+8
    bool                                  mHasBeenDelayedDeleted; // off+16
};