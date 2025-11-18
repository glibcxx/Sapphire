#include "HolographicPlatformInterface.h"

HolographicPlatform::HoloFrameOfReferenceSetupData *HolographicPlatform::HoloFrameOfReferenceSetupData::ctor() {
    using Hook = sapphire::ApiLoader<
        "\x33\xC0\x48\xC7\x41\x00\x00\x00\x00\x00\x48\x89\x01\x48\x89\x41\x00\x89\x41"_sig,
        &HolographicPlatform::HoloFrameOfReferenceSetupData::ctor>;
    return (this->*Hook::origin)();
}

HolographicPlatform *HolographicPlatform::ctor() {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        "\x48\x89\x5C\x24\x00\x48\x89\x4C\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xD9\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x01\x48\x83\xC1\x00\xE8"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x4C\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xF9\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x01\x48\x83\xC1\x00\xE8"_sig,
#endif
        &HolographicPlatform::ctor>;
    return (this->*Hook::origin)();
}