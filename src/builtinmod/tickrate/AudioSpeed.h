#pragma once

#define DLL_EXPORTS
#include <fmod/fmod_common.h>
#undef DLL_EXPORTS

void installFMODHooks();
void uninstallFMODHooks();

void UpdateResamplerMethod(FMOD_DSP_RESAMPLER resampler);

void UpdateAudioSpeed(float speedFactor);