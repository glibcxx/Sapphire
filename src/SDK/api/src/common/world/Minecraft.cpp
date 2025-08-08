#include "Minecraft.h"

#include "SDK/api/src/common/network/NetEventCallback.h"

void Minecraft::startClientGame(std::unique_ptr<NetEventCallback> legacyClientNetworkHandler) {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x48\x8B\x4C\x24\x00\x48\x85\xC9\x74\x00\x48\x8B\x01\xBA\x00\x00\x00\x00\x48\x8B\x00\xFF\x15\x00\x00\x00\x00\x90\x48\x8B\x1F"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\x8B\x00\x00\x00\x00\x48\x8B\x49\x00\xE8"_sig,
#endif
        &Minecraft::startClientGame>;
    (this->*Hook::origin)(std::move(legacyClientNetworkHandler));
}