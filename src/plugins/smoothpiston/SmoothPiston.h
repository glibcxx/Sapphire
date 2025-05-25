#pragma once

#include <thread>

class SmoothPistonPlugin {
public:
    class PistonActorTickHook;
    class SmoothMovingBlockHook;
    class SmoothPistonArmHook;

    static SmoothPistonPlugin &getInstance();

    float mTimeScale = 1.0f;

private:
    SmoothPistonPlugin();

    ~SmoothPistonPlugin();

    bool mEnableSmoothPiston = true;
    bool mEnablePistonTickOrderSeparator = false;

    // ServerLevel和ClientLevel各有一个BlockActor
    std::thread::id mRenderThread = {};

    uint32_t mCurrentOrder = 0;
    uint32_t mTotalTicked = 0;
};