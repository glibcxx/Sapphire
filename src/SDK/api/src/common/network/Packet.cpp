#include "Packet.h"

std::shared_ptr<Packet> MinecraftPackets::createPacket(MinecraftPacketIds id) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x48\x83\xBD\x00\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\xFF\x15"_sig,
        &MinecraftPackets::createPacket>;
    return Hook::origin(id);
}