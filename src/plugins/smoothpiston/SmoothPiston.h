#pragma once

#include <thread>

class Level;

class SmoothPistonPlugin {
public:
    class PistonActorUpdatePacketHook;
    class PistonActorTickHook;
    class SmoothMovingBlockHook;
    class SmoothPistonArmHook;

    static SmoothPistonPlugin &getInstance();

    std::thread::id mClientMainThread{};
    float           mTimeScale = 1.0f;

private:
    SmoothPistonPlugin();

    ~SmoothPistonPlugin();

    bool mEnableSmoothPiston = true;
    bool mEnablePistonTickOrderSeparator = false;

    uint32_t mCurrentOrder = 0;
    uint32_t mTotalTicked = 0;
};