#pragma once

#include <fmod/fmod_common.h>

void installFMODHooks();

void UpdateResamplerMethod(FMOD_DSP_RESAMPLER resampler);

void UpdateAudioSpeed(float speedFactor);