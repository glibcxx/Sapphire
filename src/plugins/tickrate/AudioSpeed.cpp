#include "AudioSpeed.h"

#include <mutex>

#include <fmod/fmod.hpp>

#include "SDK/api/sapphire/hook/Hook.h"
#include "logger/GameLogger.hpp"

/*
    这坨东西用于实现音频变速，核心思路就是：
    mcbe使用了fmod库，剩下的交给AI
*/

static FMOD::System      *gSystem = nullptr;
static FMOD_DSP_RESAMPLER currentResamplerMethod = FMOD_DSP_RESAMPLER_LINEAR;

static float gSpeedFactor = 1.0f;
static float gLastSpeedFactor = 1.0f;

static void setResamplerMethod(FMOD::System &sys, FMOD_DSP_RESAMPLER resampler) {
    FMOD_ADVANCEDSETTINGS advSettings = {};
    advSettings.cbSize = sizeof(FMOD_ADVANCEDSETTINGS);
    FMOD_RESULT getAdvRes = sys.getAdvancedSettings(&advSettings);
    if (getAdvRes != FMOD_OK) {
        Logger::Warn("[Tickrate][AudioSpeed] Failed to get original FMOD advanced settings: {}", (int)getAdvRes);
    }
    Logger::Debug("[Tickrate][AudioSpeed] resamplerMethod changed from {} to {}", (int)advSettings.resamplerMethod, (int)resampler);
    advSettings.resamplerMethod = resampler;
    FMOD_RESULT setAdvRes = sys.setAdvancedSettings(&advSettings);
    if (setAdvRes != FMOD_OK) {
        Logger::Warn("[Tickrate][AudioSpeed] Failed to set FMOD advanced resampling method: {}. This might ignore the setting.", (int)setAdvRes);
    }
}

HOOK_RAW_TYPE(
    FMODSystemInitHook,
    FMOD::System,
    hook::HookPriority::Normal,
    FMOD::System::init,
    FMOD_RESULT,
    int            maxchannels,
    FMOD_INITFLAGS flags,
    void          *extradriverdata
) {
    setResamplerMethod(*this, currentResamplerMethod);

    FMOD_RESULT res = this->origin(maxchannels, flags, extradriverdata);
    if (res == FMOD_OK) {
        gSystem = this;
    }
    return res;
}

HOOK_RAW_TYPE(
    FMODSystemPlaySoundHook,
    FMOD::System,
    hook::HookPriority::Normal,
    FMOD::System::playSound,
    FMOD_RESULT,
    FMOD::Sound        *sound,
    FMOD::ChannelGroup *channelgroup,
    bool                paused,
    FMOD::Channel     **channel
) {
    FMOD_RESULT res = this->origin(sound, channelgroup, paused, channel);
    if (res == FMOD_OK && channel && *channel) {
        float freq = 0.0f;
        if ((*channel)->getFrequency(&freq) == FMOD_OK) {
            (*channel)->setFrequency(freq * gSpeedFactor);
        }
    }
    return res;
}

void ApplySpeedToGroup(FMOD::ChannelGroup *group) {
    if (!group) return;

    int numChannels = 0;
    if (group->getNumChannels(&numChannels) != FMOD_OK)
        return;

    for (int i = 0; i < numChannels; ++i) {
        FMOD::Channel *ch = nullptr;
        if (group->getChannel(i, &ch) != FMOD_OK || !ch) continue;

        float baseFreq = 0.0f;
        if (ch->getFrequency(&baseFreq) != FMOD_OK) continue;
        if (baseFreq > 0.0f)
            ch->setFrequency(baseFreq * gSpeedFactor / gLastSpeedFactor);
    }

    // 递归子组
    int numGroups = 0;
    if (group->getNumGroups(&numGroups) != FMOD_OK)
        return;

    for (int i = 0; i < numGroups; ++i) {
        FMOD::ChannelGroup *subGroup = nullptr;
        if (group->getGroup(i, &subGroup) == FMOD_OK && subGroup)
            ApplySpeedToGroup(subGroup);
    }
}

void UpdateResamplerMethod(FMOD_DSP_RESAMPLER resampler) {
    if (!gSystem) return;
    currentResamplerMethod = resampler;
    setResamplerMethod(*gSystem, currentResamplerMethod);
}

void UpdateAudioSpeed(float speed) {
    if (!gSystem) return;
    gSpeedFactor = speed;
    FMOD::ChannelGroup *masterGroup = nullptr;
    if (gSystem->getMasterChannelGroup(&masterGroup) != FMOD_OK || !masterGroup)
        return;
    ApplySpeedToGroup(masterGroup);
    gLastSpeedFactor = gSpeedFactor;
}

// 安装 Hook
void installFMODHooks() {
    if (!FMODSystemInitHook::hook() || !FMODSystemPlaySoundHook::hook()) {
        Logger::Error("[Tickrate][AudioSpeed] FMOD Hook 安装失败!");
        FMODSystemInitHook::unhook();
        FMODSystemPlaySoundHook::unhook();
        return;
    }
}
