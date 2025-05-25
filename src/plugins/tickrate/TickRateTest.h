#pragma once

#include <vector>
#include <array>
#include <string>

#include "AudioSpeed.h"

class TickRatePlugin {
public:
    class NormalTickRateHook;
    class ParticleTickRateHook2;

    static constexpr std::array sResamplerMethodNames = {
        "DEFAULT", "NOINTERP", "LINEAR", "CUBIC", "SPLINE"
    };
    static constexpr std::array sResamplerMethods = {
        FMOD_DSP_RESAMPLER_DEFAULT,
        FMOD_DSP_RESAMPLER_NOINTERP,
        FMOD_DSP_RESAMPLER_LINEAR,
        FMOD_DSP_RESAMPLER_CUBIC,
        FMOD_DSP_RESAMPLER_SPLINE
    };

    static TickRatePlugin &getInstance();

    void setTimeScale(float scale, bool showToast = true);

private:
    TickRatePlugin();
    ~TickRatePlugin();

    void clampSelectedIndex();

    void resetTimeScale();

    void _setupSettingGUI();

    void _setupHotkeys();

    void _renderSettingGUI();

    std::vector<float> mTimeScaleList = {
        0.00625f, 0.05f, 0.1f, 0.2f, 0.25f, 0.5f, 1.0f, 2.0f, 5.0f, 10.0f
    };
    std::vector<float>     mParsedTimeScaleList;                // 从输入框解析出的列表 (临时)
    std::array<char, 1024> mTimeScaleListInputBuffer;           // 输入框的缓冲区
    std::string            mInputErrorMsg = "";                 // 输入错误信息
    bool                   mInputValid = true;                  // 输入是否有效
    bool                   mInputChangedSinceLastApply = false; // 输入框内容是否已更改

    float mTimeScale = 1.0f;
    float mTimeScaleBuffer = mTimeScale;

    int mSelectedTps = 6;

    bool mChangeAudioSpeed = true;
    int  mSelectedResamplerMethod = 2;
};