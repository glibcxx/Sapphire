#include "LegacyClientNetworkHandler.h"

void LegacyClientNetworkHandler::handle(const NetworkIdentifier &source, std::shared_ptr<UpdateBlockSyncedPacket> packet) {
    using Fn = void (LegacyClientNetworkHandler::*)(const NetworkIdentifier &, std::shared_ptr<UpdateBlockSyncedPacket>);
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        "\x40\x53\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x49\x8B\xD8\x48\x89\x5C\x24\x00\x0F\x57\xC0\x0F\x11\x44\x24\x00\x4D\x8B\x00"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x49\x8B\xF0\x4C\x8B\xFA\x48\x8B\xF9"_sig,
#endif
        (Fn)&LegacyClientNetworkHandler::handle>;
    (this->*Bind::origin)(source, std::move(packet));
}

void LegacyClientNetworkHandler::handle(const NetworkIdentifier &source, std::shared_ptr<UpdateBlockPacket> packet) {
    using Fn = void (LegacyClientNetworkHandler::*)(const NetworkIdentifier &, std::shared_ptr<UpdateBlockPacket>);
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        "\x4C\x89\x44\x24\x00\x53\x48\x83\xEC\x00\x49\x8B\xD8\x45\x33\xC9"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x4C\x89\x44\x24\x00\x57\x48\x83\xEC\x00\x49\x8B\xD8\x45\x33\xC9"_sig,
#endif
        (Fn)&LegacyClientNetworkHandler::handle>;
    (this->*Bind::origin)(source, std::move(packet));
}

void LegacyClientNetworkHandler::handle(const NetworkIdentifier &source, const BlockEventPacket &packet) {
    using Fn = void (LegacyClientNetworkHandler::*)(const NetworkIdentifier &, const BlockEventPacket &);
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x81\xEC\x00\x00\x00\x00\x49\x8B\xF8\x48\x8B\xEA"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x49\x8B\xF8\x4C\x89\x44\x24\x00\x48\x89\x54\x24"_sig,
#endif
        (Fn)&LegacyClientNetworkHandler::handle>;
    (this->*Bind::origin)(source, std::move(packet));
}

void LegacyClientNetworkHandler::handle(const NetworkIdentifier &source, std::shared_ptr<BlockActorDataPacket> packet) {
    using Fn = void (LegacyClientNetworkHandler::*)(const NetworkIdentifier &, std::shared_ptr<BlockActorDataPacket>);
    using Bind = sapphire::bind::Fn<
        "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x4C\x89\x44\x24\x18\x57\x48\x83\xEC\x70\x49\x8B\x00\x48\x8B\xF2\x48\x8B\x00\x48\x8B"_sig,
        (Fn)&LegacyClientNetworkHandler::handle>;
    (this->*Bind::origin)(source, std::move(packet));
}

void LegacyClientNetworkHandler::handle(const NetworkIdentifier &source, std::shared_ptr<LevelChunkPacket> packet) {
    /*
        search "deserialize LevelChunkPacket"
    */
    using Fn = void (LegacyClientNetworkHandler::*)(const NetworkIdentifier &, std::shared_ptr<LevelChunkPacket>);
    using Bind = sapphire::bind::Fn<
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x00\x8B\x00\x48\x8B\xF2\x4C\x8B\xF9\x4C\x89\x85"_sig,
        (Fn)&LegacyClientNetworkHandler::handle>;
    (this->*Bind::origin)(source, std::move(packet));
}

void LegacyClientNetworkHandler::handle(const NetworkIdentifier &source, const class UpdateSubChunkBlocksPacket &packet) {
    using Fn = void (LegacyClientNetworkHandler::*)(const NetworkIdentifier &, const class UpdateSubChunkBlocksPacket &);
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x81\xEC\x00\x00\x00\x00\x49\x8B\xD8\x48\x8B\xF2\x48\x8B\xF9"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x4D\x8B\xF0\x4C\x8B\xE2\x4C\x8B\xF9"_sig,
#endif
        (Fn)&LegacyClientNetworkHandler::handle>;
    (this->*Bind::origin)(source, std::move(packet));
}