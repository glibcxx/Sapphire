#pragma once

#include <thread>

class Level;

class SmoothPistonMod {
public:
    class PistonActorUpdatePacketHook;
    class PistonActorTickHook;
    class SmoothMovingBlockHook;
    class SmoothPistonArmHook;

    static SmoothPistonMod &getInstance();

    std::thread::id mClientMainThread{};
    float           mTimeScale = 1.0f;

private:
    SmoothPistonMod();

    ~SmoothPistonMod();

    bool mEnableSmoothPiston = true;
    bool mEnablePistonTickOrderSeparator = false;

    uint32_t mCurrentOrder = 0;
    uint32_t mTotalTicked = 0;
};