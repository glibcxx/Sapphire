#include "TickRateTest.h"

#include "SDK/core/Core.h"

#include "SDK/api/src/common/util/Timer.h"
#include "SDK/api/sapphire/GUI/GUI.h"

#include "AudioSpeed.h"

constexpr float TimeScaleList[] = {0.00625f, 0.05f, 0.1f, 0.2f, 0.25f, 0.5f, 1.0f, 2.0f, 5.0f, 10.0f};

static float   gTimeScale = 1.0f;
static uint8_t sSelectedTps = 6;

HOOK_TYPE(TickRateTest, Timer, Timer::advanceTime, void, float preferredFrameStep) {
    this->origin(preferredFrameStep);
    this->mTimeScale = gTimeScale; // 就这么写，很随意2333
}

void addSettingGUI() {
    static float   timeScale = gTimeScale;
    PluginSettings myPluginSettings{
        "Tick Rate",
        "Change game Speed!",
        []() {
            ImGui::SliderFloat("TickSpeed", &timeScale, 0.0f, 10.0f);
            if (ImGui::Button("Apply Settings")) {
                gTimeScale = timeScale;
            }
        }
    };
    GuiOverlay::registerPluginSettings(std::move(myPluginSettings));
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
