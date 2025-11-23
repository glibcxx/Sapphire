#include "ToastMessage.h"

ToastMessage *ToastMessage::ctor(
    ToastMessageType   type,
    const std::string &title,
    const std::string &subtitle,
    const Json::Value &propertyBag,
    float              a5
) {
    using Fn = ToastMessage *(ToastMessage::*)(ToastMessageType, const std::string &, const std::string &, const Json::Value &, float);
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x4C\x24\x00\x56\x57\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x00\x49\x8B\xD9\x49\x8B\xC0"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x4C\x24\x00\x57\x41\x56\x41\x57\x48\x83\xEC\x00\x49\x8B\xD9\x8B\xEA"_sig,
#endif
        (Fn)&ToastMessage::ctor>;
    return (this->*Bind::origin)(type, title, subtitle, propertyBag, a5);
}