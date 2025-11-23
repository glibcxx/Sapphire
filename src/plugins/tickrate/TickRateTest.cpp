#include "TickRateTest.h"

#include "SDK/api/sapphire/hook/Hook.h"

#include "SDK/api/sapphire/GUI/GUI.h"
#include "SDK/api/src/common/util/Timer.h"
#include "SDK/api/src-client/common/client/particlesystem/particle/ParticleEmitter.h"

#include "../smoothpiston/SmoothPiston.h"

static TickRatePlugin  *plugin = nullptr;
static sapphire::Logger sLogger{"TickRate"};

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

void TickRatePlugin::clampSelectedIndex() {
    if (mTimeScaleList.empty()) {
        mSelectedTps = 0;
    } else {
        mSelectedTps = std::clamp<int>(mSelectedTps, 0, mTimeScaleList.size() - 1);
    }
    if (!mTimeScaleList.empty()) {
        mTimeScaleBuffer = mTimeScaleList[mSelectedTps];
    } else {
        mTimeScaleBuffer = 1.0f;
    }
}

void TickRatePlugin::setTimeScale(float scale, bool showToast) {
    mTimeScaleBuffer = mTimeScale = scale;
    if (mChangeAudioSpeed)
        UpdateAudioSpeed(mTimeScale);
    SmoothPistonPlugin::getInstance().mTimeScale = mTimeScale;
    if (showToast)
        GuiOverlay::addToast(std::format("TickSpeed: {} Tps (x{})", mTimeScale * 20, mTimeScale));
}

void TickRatePlugin::resetTimeScale() {
    auto it = std::find(mTimeScaleList.begin(), mTimeScaleList.end(), 1.0f);
    if (it != mTimeScaleList.end()) {
        mSelectedTps = static_cast<uint8_t>(std::distance(mTimeScaleList.begin(), it));
    } else if (!mTimeScaleList.empty()) {
        mSelectedTps = (uint8_t)(mTimeScaleList.size() / 2);
    } else {
        mSelectedTps = 0;
    }
    this->clampSelectedIndex();
    this->setTimeScale(mTimeScaleList.empty() ? 1.0f : mTimeScaleList[mSelectedTps]);
}

void TickRatePlugin::_renderSettingGUI() {
    bool changed = false;
    if (mTimeScaleList.empty()) {
        ImGui::BeginDisabled(true);
        ImGui::SliderInt("##TickSpeedSetting", nullptr, 0, 0, "TimeScale list is empty!");
        ImGui::EndDisabled();
    } else if ((changed = ImGui::SliderInt(
                    "##TickSpeedSetting",
                    &mSelectedTps,
                    0,
                    mTimeScaleList.size() - 1,
                    std::format(
                        "TickSpeed: {} Tps (x{})",
                        mTimeScaleList[mSelectedTps] * 20,
                        mTimeScaleList[mSelectedTps]
                    )
                        .data()
                ))) {
        mTimeScaleBuffer = mTimeScaleList[mSelectedTps];
    }

    ImGui::SeparatorText("Edit Speed List");

    bool oldInvalid = mInputValid;
    if (!mInputValid) {
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    }
    if (ImGui::InputText("##SpeedListInput", mTimeScaleListInputBuffer.data(), mTimeScaleListInputBuffer.size())) {
        mInputChangedSinceLastApply = true;
        if (parseTimeScaleString(mTimeScaleListInputBuffer.data(), mParsedTimeScaleList, mInputErrorMsg)) {
            mInputValid = true;
            auto it = std::find(mParsedTimeScaleList.begin(), mParsedTimeScaleList.end(), mTimeScaleBuffer);
            if (it != mParsedTimeScaleList.end()) {
                mSelectedTps = static_cast<int>(std::distance(mParsedTimeScaleList.begin(), it));
            } else {
                // 原来的值不在新列表里
                mSelectedTps = std::clamp(mSelectedTps, 0, static_cast<int>(mParsedTimeScaleList.size() - 1));
                mTimeScaleBuffer = mParsedTimeScaleList[mSelectedTps];
            }
        }
    }
    if (!oldInvalid) {
        ImGui::PopStyleColor();
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Error: %s", mInputErrorMsg.c_str());
    }
    bool list_content_changed = mInputValid && (mParsedTimeScaleList != mTimeScaleList);
    bool buffer_changed = std::fabs(mTimeScale - mTimeScaleBuffer) > 1e-6f;
    bool can_apply = mInputValid && (mInputChangedSinceLastApply && list_content_changed || changed || buffer_changed);
    ImGui::BeginDisabled(!can_apply);
    if (ImGui::Button("Apply")) {
        if (mInputChangedSinceLastApply && list_content_changed) {
            mTimeScaleList = mParsedTimeScaleList;
            clampSelectedIndex();
            mInputChangedSinceLastApply = false;
        }
        mTimeScale = mTimeScaleBuffer;
        if (mChangeAudioSpeed)
            UpdateAudioSpeed(mTimeScale);
    }
    ImGui::EndDisabled();

    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
        resetTimeScale();
    }
    ImGui::SeparatorText("Audio Setting");

    bool audioSpeedChanged = ImGui::Checkbox("Change Audio Speed", &mChangeAudioSpeed);
    if (audioSpeedChanged) {
        UpdateAudioSpeed(mChangeAudioSpeed ? mTimeScale : 1.0f);
    }

    // 设置重采样算法
    if (ImGui::Combo(
            "Resampler Method", &mSelectedResamplerMethod, sResamplerMethodNames.data(), sResamplerMethodNames.size()
        )) {
        if (mSelectedResamplerMethod >= 0 && mSelectedResamplerMethod < sResamplerMethods.size()) {
            UpdateResamplerMethod(sResamplerMethods[mSelectedResamplerMethod]);
        }
    }
}

void TickRatePlugin::_setupSettingGUI() {
    GuiOverlay::PluginSettings tickrateSettings{
        "Tick Rate",
        "Change game Speed!",
        [this]() {
            _renderSettingGUI();
        }
    };
    GuiOverlay::registerPluginSettings(std::move(tickrateSettings));
}

void TickRatePlugin::_setupHotkeys() {
    GuiOverlay::registerHotkey(
        {.keysDown = {ImGuiMod_Alt},
         .triggerKey = ImGuiKey_KeypadAdd,
         .action = [this]() {
             if (++mSelectedTps > mTimeScaleList.size() - 1)
                 mSelectedTps = mTimeScaleList.size() - 1;
             this->setTimeScale(mTimeScaleList[mSelectedTps]);
         }}
    );
    GuiOverlay::registerHotkey(
        {.keysDown = {ImGuiMod_Alt},
         .triggerKey = ImGuiKey_KeypadSubtract,
         .action = [this]() {
             if (mSelectedTps > 0)
                 --mSelectedTps;
             this->setTimeScale(mTimeScaleList[mSelectedTps]);
         }}
    );
    GuiOverlay::registerHotkey(
        {.keysDown = {ImGuiMod_Alt},
         .triggerKey = ImGuiKey_KeypadDecimal,
         .action = [this]() {
             this->resetTimeScale();
         }}
    );
}

HOOK_TYPE(TickRatePlugin::NormalTickRateHook, Timer, sapphire::hook::HookPriority::Normal, Timer::advanceTime, void, float preferredFrameStep) {
    origin(preferredFrameStep);
    mTimeScale = plugin->mTimeScale;
}

HOOK_TYPE(
    TickRatePlugin::ParticleTickRateHook2,
    ParticleSystem::ParticleEmitterActual,
    sapphire::hook::HookPriority::Normal,
    ParticleSystem::ParticleEmitterActual::tick,
    void,
    const std::chrono::nanoseconds &dtIn,
    float                           a
) {
    // sLogger.debug("[{}] dtIn: {}, alpha: {:.6f}", std::chrono::steady_clock::now().time_since_epoch().count(), dtIn.count(), a);
    origin(std::chrono::duration_cast<std::chrono::nanoseconds>(dtIn * plugin->mTimeScale), a);
}

TickRatePlugin::TickRatePlugin() {
    plugin = this;
    if (NormalTickRateHook::hook() && ParticleTickRateHook2::hook()) {
        installFMODHooks();
        this->_setupSettingGUI();
        this->_setupHotkeys();

        convertListToString(mTimeScaleList, mTimeScaleListInputBuffer.data(), mTimeScaleListInputBuffer.size());
        mParsedTimeScaleList = mTimeScaleList;
        this->clampSelectedIndex();
        mTimeScale = mTimeScaleBuffer;
        UpdateAudioSpeed(mTimeScale);
        UpdateResamplerMethod(sResamplerMethods[mSelectedResamplerMethod]);
        mInputValid = true;
        mInputErrorMsg.clear();
        mInputChangedSinceLastApply = false;
        sLogger.debug("Tickrate 安装成功！");
    } else {
        sLogger.error("Tickrate 安装失败！");
    }
}

TickRatePlugin::~TickRatePlugin() {
    uninstallFMODHooks();
    ParticleTickRateHook2::unhook();
    NormalTickRateHook::unhook();
}

TickRatePlugin &TickRatePlugin::getInstance() {
    static TickRatePlugin t{};
    return t;
}
