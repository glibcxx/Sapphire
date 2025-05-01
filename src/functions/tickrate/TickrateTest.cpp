#include "TickRateTest.h"

#include "SDK/core/Core.h"

#include "SDK/api/src/common/util/Timer.h"

#include "../guioverlay/GuiOverlay.h"
#include "AudioSpeed.h"

static float gTimeScale = 1.0f;

HOOK_TYPE(TickRateTest, Timer, Timer::advanceTime, void, float preferredFrameStep) {
    this->origin(preferredFrameStep);
    this->mTimeScale = GuiOverlay::sTimeScale; // 就这么写，很随意2333
}

void addSettingGUI() {
    PluginSettings myPluginSettings;
    myPluginSettings.name = "Tick Rate";
    myPluginSettings.description = "Change game Speed!";
    myPluginSettings.drawSettings = []() {
        ImGui::SliderFloat("TickSpeed", &gTimeScale, 0.0f, 10.0f);
        if (ImGui::Button("Apply Settings")) {
            GuiOverlay::sTimeScale = gTimeScale;
        }
    };
    GuiOverlay::registerPluginSettings(myPluginSettings);
}

void installTickRate() {
    if (TickRateTest::hook()) {
        installFMODHooks();
        addSettingGUI();
    } else {
        Logger::ErrorBox(L"Tickrate 安装失败！");
    }
}

void uninstallTickRate() {
    TickRateTest::unhook();
}
