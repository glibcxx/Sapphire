#include "TickRateTest.h"

#include "SDK/core/Core.h"

#include "SDK/api/src/common/util/Timer.h"

#include "../guioverlay/GuiOverlay.h"
#include "AudioSpeed.h"

HOOK_TYPE(TickRateTest, Timer, Timer::advanceTime, void, float preferredFrameStep) {
    this->origin(preferredFrameStep);
    this->mTimeScale = GuiOverlay::sTimeScale; // 就这么写，很随意2333
}

void installTickRate() {
    if (TickRateTest::hook())
        installFMODHooks();
    else
        LogBox::Error(L"Tickrate 安装失败！");
}

void uninstallTickRate() {
    TickRateTest::unhook();
}
