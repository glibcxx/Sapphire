#include "Minecraft.h"

#include "SDK/api/src/common/network/NetEventCallback.h"

void Minecraft::startClientGame(std::unique_ptr<NetEventCallback> legacyClientNetworkHandler) {
    using Hook = sapphire::ApiLoader<
        "\xE8\x00\x00\x00\x00\x90\x48\x8B\x4C\x24\x00\x48\x85\xC9\x74\x00\x48\x8B\x01\x8D\x55"_sig,
        &Minecraft::startClientGame,
        sapphire::deRefCall>;
    (this->*Hook::origin)(std::move(legacyClientNetworkHandler));
}