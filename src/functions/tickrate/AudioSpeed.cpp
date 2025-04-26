#include "AudioSpeed.h"

#include <mutex>

#include <fmod/fmod.hpp>

#include "hook/Hook.hpp"
#include "logger/LogBox.hpp"

#include "../guioverlay/GuiOverlay.h"

/*
    这坨东西用于实现音频变速，核心思路就是：
    mcbe使用了fmod库，剩下的交给AI
*/

static FMOD::System *gSystem = nullptr;
static FMOD::DSP    *gPitchDSP = nullptr;
static std::mutex    gDspMutex;

float gSpeedFactor = 1.0f;
float gLastSpeedFactor = 1.0f;

typedef FMOD_RESULT(F_CALL *PFN_FMOD_System_Init)(FMOD::System *system, int maxchannels, FMOD_INITFLAGS flags, void *extradriverdata);
PFN_FMOD_System_Init oSystemInit = nullptr;
typedef FMOD_RESULT(F_CALL *PFN_FMOD_System_playSound)(
    FMOD::System       *self,
    FMOD::Sound        *sound,
    FMOD::ChannelGroup *group,
    bool                paused,
    FMOD::Channel     **channel);
PFN_FMOD_System_playSound oPlaySound = nullptr;

FMOD_RESULT F_CALL hkSystemInit(FMOD::System *ppSystem, int maxchannels, FMOD_INITFLAGS flags, void *extradata) {
    FMOD_RESULT res = oSystemInit(ppSystem, maxchannels, flags, extradata);
    if (res == FMOD_OK && ppSystem) {
        std::lock_guard<std::mutex> lk(gDspMutex);
        gSystem = ppSystem;

        // 1. 创建 PitchShift DSP
        if (!gPitchDSP) {
            gSystem->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &gPitchDSP);
            gPitchDSP->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 1.0f);

            // 2. 把 DSP 挂到 Master Channel Group
            FMOD::ChannelGroup *masterGroup = nullptr;
            gSystem->getMasterChannelGroup(&masterGroup);
            masterGroup->addDSP(0, gPitchDSP);
        }
    }
    return res;
}

FMOD_RESULT F_CALL hkPlaySound(
    FMOD::System       *self,
    FMOD::Sound        *sound,
    FMOD::ChannelGroup *group,
    bool                paused,
    FMOD::Channel     **channel) {
    FMOD_RESULT res = oPlaySound(self, sound, group, paused, channel);

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

void UpdateAudioSpeed(float speed) {
    gSpeedFactor = speed;
    if (!gSystem) return;
    FMOD::ChannelGroup *masterGroup = nullptr;
    if (gSystem->getMasterChannelGroup(&masterGroup) != FMOD_OK || !masterGroup)
        return;
    ApplySpeedToGroup(masterGroup);
    gLastSpeedFactor = gSpeedFactor;
}

// 安装 Hook
void installFMODHooks() {
    HMODULE hFMOD = GetModuleHandleW(L"fmod.dll");
    if (!hFMOD) return;

    auto createAddr = (PFN_FMOD_System_Init)GetProcAddress(hFMOD, "?init@System@FMOD@@QEAA?AW4FMOD_RESULT@@HIPEAX@Z");
    if (!createAddr)
        LogBox::Error(L"FMOD::System::init 未找到!");
    auto playSoundAddr = (PFN_FMOD_System_Init)GetProcAddress(hFMOD, "?playSound@System@FMOD@@QEAA?AW4FMOD_RESULT@@PEAVSound@2@PEAVChannelGroup@2@_NPEAPEAVChannel@2@@Z");
    if (!playSoundAddr)
        LogBox::Error(L"FMOD::System::playSound 未找到!");

    oSystemInit = hook::hookFunc((PFN_FMOD_System_Init)createAddr, hkSystemInit);
    oPlaySound = hook::hookFunc((PFN_FMOD_System_playSound)playSoundAddr, hkPlaySound);
    if (!oSystemInit || !oPlaySound) {
        LogBox::Error(L"FMOD Hook 安装失败!");
        hook::unhookFunc(createAddr);
        hook::unhookFunc(playSoundAddr);
        return;
    }
}
