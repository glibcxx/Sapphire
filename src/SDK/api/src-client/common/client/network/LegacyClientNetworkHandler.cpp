#include "LegacyClientNetworkHandler.h"

/*
    search "deserialize LevelChunkPacket" to find
    LegacyClientNetworkHandler::handle(const NetworkIdentifier &source, std::shared_ptr<LevelChunkPacket> packet)
*/

void LegacyClientNetworkHandler::handle(
    const NetworkIdentifier                 &source,
    std::shared_ptr<UpdateBlockSyncedPacket> packet
) {
    using Fn = void (LegacyClientNetworkHandler::*)(const NetworkIdentifier &, std::shared_ptr<UpdateBlockSyncedPacket>);
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        "\x40\x53\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x49\x8B\xD8\x48\x89\x5C\x24\x00\x0F\x57\xC0\x0F\x11\x44\x24\x00\x4D\x8B\x00"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x49\x8B\xF0\x4C\x8B\xFA\x48\x8B\xF9"_sig,
#endif
        (Fn)&LegacyClientNetworkHandler::handle>;
    (this->*Hook::origin)(source, std::move(packet));
}

void LegacyClientNetworkHandler::handle(
    const NetworkIdentifier           &source,
    std::shared_ptr<UpdateBlockPacket> packet
) {
    using Fn = void (LegacyClientNetworkHandler::*)(const NetworkIdentifier &, std::shared_ptr<UpdateBlockPacket>);
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        "\x4C\x89\x44\x24\x00\x53\x48\x83\xEC\x00\x49\x8B\xD8\x45\x33\xC9"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x4C\x89\x44\x24\x00\x57\x48\x83\xEC\x00\x49\x8B\xD8\x45\x33\xC9"_sig,
#endif
        (Fn)&LegacyClientNetworkHandler::handle>;
    (this->*Hook::origin)(source, std::move(packet));
}

void LegacyClientNetworkHandler::handle(
    const NetworkIdentifier              &source,
    std::shared_ptr<BlockActorDataPacket> packet
) {
    using Fn = void (LegacyClientNetworkHandler::*)(const NetworkIdentifier &, std::shared_ptr<BlockActorDataPacket>);
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x4C\x89\x44\x24\x18\x57\x48\x83\xEC\x70\x49\x8B\x00\x48\x8B\xF2\x48\x8B\x00\x48\x8B"_sig,
        (Fn)&LegacyClientNetworkHandler::handle>;
    (this->*Hook::origin)(source, std::move(packet));
}