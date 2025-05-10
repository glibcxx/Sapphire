#include "TickRateTest.h"

#include "SDK/core/Core.h"

#include "SDK/api/src/common/util/Timer.h"
#include "SDK/api/sapphire/GUI/GUI.h"
#include "SDK/api/src-client/common/client/particlesystem/particle/ParticleEmitter.h"

#include "logger/GameLogger.hpp"

#include "AudioSpeed.h"

constexpr float TimeScaleList[] = {0.00625f, 0.05f, 0.1f, 0.2f, 0.25f, 0.5f, 1.0f, 2.0f, 5.0f, 10.0f};

static std::vector<float> gTimeScaleList = {
    0.00625f, 0.05f, 0.1f, 0.2f, 0.25f, 0.5f, 1.0f, 2.0f, 5.0f, 10.0f
};
static std::vector<float> gParsedTimeScaleList;                // 从输入框解析出的列表 (临时)
static char               gTimeScaleListInputBuffer[1024];     // 输入框的缓冲区
static std::string        gInputErrorMsg = "";                 // 输入错误信息
static bool               gInputValid = true;                  // 输入是否有效
static bool               gInputChangedSinceLastApply = false; // 输入框内容是否已更改

static float gTimeScale = 1.0f;
static float gTimeScaleBuffer = gTimeScale;

static int sSelectedTps = 6;

static bool changeAudioSpeed = true;

static void clampSelectedIndex() {
    if (gTimeScaleList.empty()) {
        sSelectedTps = 0;
    } else {
        sSelectedTps = std::clamp<int>(sSelectedTps, 0, gTimeScaleList.size() - 1);
    }
    if (!gTimeScaleList.empty()) {
        gTimeScaleBuffer = gTimeScaleList[sSelectedTps];
    } else {
        gTimeScaleBuffer = 1.0f;
    }
}

static void convertListToString(const std::vector<float> &list, char *out_buf, size_t buf_size) {
    if (list.empty()) {
        out_buf[0] = '\0';
        return;
    }
    std::string temp_str;
    for (size_t i = 0; i < list.size(); ++i) {
        std::format_to(std::back_inserter(temp_str), "{};", list[i]);
    }
    temp_str.back() = '\0';
    std::strncpy(out_buf, temp_str.c_str(), temp_str.size());
}

static bool parseTimeScaleString(const char *input, std::vector<float> &out_list, std::string &out_error) {
    out_list.clear();
    out_error.clear();
    std::string       segment;
    std::stringstream ss(input);

    bool containOne = false;
    while (std::getline(ss, segment, ';')) {
        std::erase_if(segment, isspace);
        if (segment.empty())
            continue;

        try {
            size_t processed_chars;
            float  value = std::stof(segment, &processed_chars);
            if (processed_chars != segment.length()) {
                std::format_to(std::back_inserter(out_error), "Invalid characters in segment: '{}'", segment);
                return false;
            }
            if (value <= 0.0f) {
                std::format_to(std::back_inserter(out_error), "Value must be positive: {}", value);
                return false;
            }
            if (value > 100.0f) {
                std::format_to(std::back_inserter(out_error), "Value too large: {}", value);
                return false;
            }
            if (value == 1.0f)
                containOne = true;
            out_list.push_back(value);
        } catch (const std::invalid_argument &e) {
            std::format_to(std::back_inserter(out_error), "Invalid number format: '{}'", segment);
            return false;
        } catch (const std::out_of_range &e) {
            std::format_to(std::back_inserter(out_error), "Number out of range: '{}'", segment);
            return false;
        }
    }
    if (out_list.empty()) {
        out_error = "List cannot be empty.";
        return false;
    }
    if (!containOne) {
        out_error = "List must contain 1.0 or 1.";
        return false;
    }
    return true;
}

static void selectNextTimeScale() {
    if (++sSelectedTps > gTimeScaleList.size() - 1)
        sSelectedTps = gTimeScaleList.size() - 1;
    gTimeScaleBuffer = gTimeScale = TimeScaleList[sSelectedTps];
    UpdateAudioSpeed(gTimeScale);
}

static void selectLastTimeScale() {
    if (sSelectedTps > 0)
        --sSelectedTps;
    gTimeScaleBuffer = gTimeScale = TimeScaleList[sSelectedTps];
    if (changeAudioSpeed)
        UpdateAudioSpeed(gTimeScale);
}

static void resetTimeScale() {
    auto it = std::find(gTimeScaleList.begin(), gTimeScaleList.end(), 1.0f);
    if (it != gTimeScaleList.end()) {
        sSelectedTps = static_cast<uint8_t>(std::distance(gTimeScaleList.begin(), it));
    } else if (!gTimeScaleList.empty()) {
        sSelectedTps = (uint8_t)(gTimeScaleList.size() / 2);
    } else {
        sSelectedTps = 0;
    }
    clampSelectedIndex();
    gTimeScaleBuffer = gTimeScale = gTimeScaleList.empty() ? 1.0f : gTimeScaleList[sSelectedTps];
    if (changeAudioSpeed)
        UpdateAudioSpeed(gTimeScale);
}

static void settingGUI() {
    bool changed = false;
    if (gTimeScaleList.empty()) {
        ImGui::BeginDisabled(true);
        ImGui::SliderInt("##TickSpeedSetting", nullptr, 0, 0, "TimeScale list is empty!");
        ImGui::EndDisabled();
    } else if (changed = ImGui::SliderInt(
                   "##TickSpeedSetting",
                   &sSelectedTps,
                   0,
                   gTimeScaleList.size() - 1,
                   std::format(
                       "TickSpeed: {} Tps (x{})",
                       gTimeScaleList[sSelectedTps] * 20,
                       gTimeScaleList[sSelectedTps]
                   )
                       .data()
               )) {
        gTimeScaleBuffer = gTimeScaleList[sSelectedTps];
    }

    ImGui::SeparatorText("Edit Speed List");

    bool oldInvalid = gInputValid;
    if (!gInputValid) {
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    }
    if (ImGui::InputText("##SpeedListInput", gTimeScaleListInputBuffer, sizeof(gTimeScaleListInputBuffer))) {
        gInputChangedSinceLastApply = true;
        if (gInputValid = parseTimeScaleString(gTimeScaleListInputBuffer, gParsedTimeScaleList, gInputErrorMsg)) {
            auto it = std::find(gParsedTimeScaleList.begin(), gParsedTimeScaleList.end(), gTimeScaleBuffer);
            if (it != gParsedTimeScaleList.end()) {
                sSelectedTps = static_cast<int>(std::distance(gParsedTimeScaleList.begin(), it));
            } else {
                // 原来的值不在新列表里
                sSelectedTps = std::clamp(sSelectedTps, 0, static_cast<int>(gParsedTimeScaleList.size() - 1));
                gTimeScaleBuffer = gParsedTimeScaleList[sSelectedTps];
            }
        }
    }
    if (!oldInvalid) {
        ImGui::PopStyleColor();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Error: %s", gInputErrorMsg.c_str());
    }
    bool list_content_changed = gInputValid && (gParsedTimeScaleList != gTimeScaleList);
    bool buffer_changed = std::fabs(gTimeScale - gTimeScaleBuffer) > 1e-6f;
    bool can_apply = gInputValid && ((gInputChangedSinceLastApply && list_content_changed) || changed || buffer_changed);
    ImGui::BeginDisabled(!can_apply);
    if (ImGui::Button("Apply")) {
        if (gInputChangedSinceLastApply && list_content_changed) {
            gTimeScaleList = gParsedTimeScaleList;
            clampSelectedIndex();
            gInputChangedSinceLastApply = false;
        }
        gTimeScale = gTimeScaleBuffer;
        if (changeAudioSpeed)
            UpdateAudioSpeed(gTimeScale);
    }
    ImGui::EndDisabled();

    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
        resetTimeScale();
    }
    ImGui::SeparatorText("Audio Setting");

    if (ImGui::Checkbox("Change Audio Speed", &changeAudioSpeed)) {
        UpdateAudioSpeed(changeAudioSpeed ? gTimeScale : 1.0f);
    }
}

void addSettingGUI() {
    GuiOverlay::PluginSettings tickrateSettings{
        "Tick Rate",
        "Change game Speed!",
        settingGUI
    };
    GuiOverlay::registerPluginSettings(std::move(tickrateSettings));
}

void addHotkeys() {
    GuiOverlay::registerHotkey({.keyChord = ImGuiMod_Alt | ImGuiKey_KeypadAdd, .action = selectNextTimeScale});
    GuiOverlay::registerHotkey({.keyChord = ImGuiMod_Alt | ImGuiKey_KeypadSubtract, .action = selectLastTimeScale});
    GuiOverlay::registerHotkey({.keyChord = ImGuiMod_Alt | ImGuiKey_KeypadDecimal, .action = resetTimeScale});
}

HOOK_TYPE(TickRateTest, Timer, Timer::advanceTime, void, float preferredFrameStep) {
    this->origin(preferredFrameStep);
    this->mTimeScale = gTimeScale;
}

HOOK_TYPE(
    TickRateTest2,
    ParticleSystem::ParticleEmitterActual,
    ParticleSystem::ParticleEmitterActual::tick,
    void,
    const std::chrono::nanoseconds &dtIn,
    float                           a
) {
    Logger::Debug("[{}] dtIn: {}, alpha: {:.6f}", std::chrono::steady_clock::now().time_since_epoch().count(), dtIn.count(), a);
    this->origin(std::chrono::duration_cast<std::chrono::nanoseconds>(dtIn * gTimeScale), a);
}

void installTickRate() {
    if (TickRateTest::hook() && TickRateTest2::hook()) {
        installFMODHooks();
        addSettingGUI();
        addHotkeys();

        convertListToString(gTimeScaleList, gTimeScaleListInputBuffer, sizeof(gTimeScaleListInputBuffer));
        gParsedTimeScaleList = gTimeScaleList;
        clampSelectedIndex();
        gTimeScale = gTimeScaleBuffer;
        UpdateAudioSpeed(gTimeScale);
        gInputValid = true;
        gInputErrorMsg.clear();
        gInputChangedSinceLastApply = false;
    } else {
        Logger::ErrorBox(L"Tickrate 安装失败！");
    }
}

void uninstallTickRate() {
    TickRateTest::unhook();
    TickRateTest2::unhook();
}
