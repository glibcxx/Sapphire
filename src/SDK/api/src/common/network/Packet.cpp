#include "Packet.h"

Bedrock::Result<void> Packet::checkSize(size_t packetSize, bool receiverIsServer) const {
    using Hook = sapphire::ApiLoader<
        "\x4C\x8B\xDC\x4D\x89\x43\x00\x53\x48\x81\xEC"_sig,
        &Packet::checkSize,
        SPHR_FUNCDNAME>;
    return (this->*Hook::origin)(packetSize, receiverIsServer);
}

Bedrock::Result<void> Packet::readNoHeader(ReadOnlyBinaryStream &bitstream, const SubClientId &subid) {
    using Hook = sapphire::ApiLoader<
        [](uintptr_t addr) { return memory::deRef(addr + 7, memory::AsmOperation::CALL); }
            | "\x48\x8D\x55\x00\x49\x8B\x0F\xE8\x00\x00\x00\x00\x48\x8B\xD0"_sig,
        &Packet::readNoHeader>;
    return (this->*Hook::origin)(bitstream, subid);
}

std::shared_ptr<Packet> MinecraftPackets::createPacket(MinecraftPacketIds id) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x48\x83\xBD\x00\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\xFF\x15"_sig,
        &MinecraftPackets::createPacket>;
    return Hook::origin(id);
}