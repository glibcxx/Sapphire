#include "Timer.h"

void Timer::advanceTime(float preferredFrameStep) {
    (this->*core::ApiLoader<"\x40\x53\x48\x83\xEC\x30\x48\x8B\xD9\x0F\x29\x74\x24\x20\x8B"_sig, &Timer::advanceTime>::origin)(preferredFrameStep);
}
