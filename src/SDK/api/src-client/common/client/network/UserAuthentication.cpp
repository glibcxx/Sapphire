#include "UserAuthentication.h"

UserAuthentication *UserAuthentication::ctor(
    std::weak_ptr<IClientInstance> client,
    uint64_t                       clientRandomId,
    const std::string             &a3
) {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2 || MC_VERSION == v1_21_50
        "\x48\x8B\xC4\x48\x89\x58\x00\x48\x89\x50\x00\x48\x89\x48\x00\x55\x56\x57\x41\x56"_sig,
#elif MC_VERSION == v1_21_60
        "\x48\x89\x54\x24\x00\x48\x89\x4C\x24\x00\x53\x55\x56\x57\x41\x56\x48\x83\xEC\x00\x49\x8B\xD9\x49\x8B\xE8"_sig,
#endif
        &UserAuthentication::ctor>;
    return (this->*Hook::origin)(std::move(client), clientRandomId, a3);
}