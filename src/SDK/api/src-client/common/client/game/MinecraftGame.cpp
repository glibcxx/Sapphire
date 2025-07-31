#include "MinecraftGame.h"

void MinecraftGame::update() {
    using Hook = sapphire::ApiLoader<
        "\x40\x53\x48\x83\xEC\x00\x48\x8B\xD9\xE8\x00\x00\x00\x00\x48\x8B\x8B\x00\x00\x00\x00\xBA"_sig,
        &MinecraftGame::update>;
    return (this->*Hook::origin)();
}

void MinecraftGame::startFrame() {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        "\xE8\x00\x00\x00\x00\x80\xBE\x00\x00\x00\x00\x00\x74\x00\x48\x8B\x9E"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\xE8\x00\x00\x00\x00\x41\x80\xBF\x00\x00\x00\x00\x00\x74\x00\x49\x8B\x9F"_sig,
#endif
        &MinecraftGame::startFrame,
        sapphire::deRefCall>;
    return (this->*Hook::origin)();
}

void MinecraftGame::endFrame() {
    using Hook = sapphire::ApiLoader<
        "\x8B\x00\xE8\x00\x00\x00\x00\x00\x8B\x00\x00\x00\x00\x00\xC6\x80\xB8\x00\x00\x00\x00\x00\x8B"_sig,
        &MinecraftGame::endFrame,
        [](uintptr_t addr) { return memory::deRef(addr + 2, memory::AsmOperation::CALL); }>;
    return (this->*Hook::origin)();
}

void MinecraftGame::updateGraphics(IClientInstance &client, const Timer &timer) {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        "\xE8\x00\x00\x00\x00\x90\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\x46\x00\x48\x8D\x95"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\xE8\x00\x00\x00\x00\x90\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x49\x8D\x4F\x00\x49\x8B\x47\x00\x48\x8D\x95\x00\x00\x00\x00\x48\x8B\x40\x00\xFF\x15\x00\x00\x00\x00\x90\x4D\x8B\xC4"_sig,
#endif
        &MinecraftGame::updateGraphics,
        sapphire::deRefCall>;
    return (this->*Hook::origin)(client, timer);
}

void MinecraftGame::updateAudio(IClientInstance &client, const Timer &timer) {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        "\xE8\x00\x00\x00\x00\x90\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x0F\xB6\x44\x24\x00\x88\x86"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\xE8\x00\x00\x00\x00\x90\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x0F\xB6\x44\x24\x00\x41\x88\x87"_sig,
#endif
        &MinecraftGame::updateAudio,
        sapphire::deRefCall>;
    return (this->*Hook::origin)(client, timer);
}

void MinecraftGame::_update() {
    using Hook = sapphire::ApiLoader<
        "\xE8\x00\x00\x00\x00\x48\x8B\x8B\x00\x00\x00\x00\xBA\x00\x00\x00\x00\x48\x83\xC4"_sig,
        &MinecraftGame::_update,
        sapphire::deRefCall>;
    return (this->*Hook::origin)();
}

bool MinecraftGame::_clientUpdate() {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        "\xE8\x00\x00\x00\x00\x44\x0F\xB6\xF0\x88\x44\x24\x00\x80\xBE"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\xE8\x00\x00\x00\x00\x0F\xB6\xF8\x88\x44\x24\x00\x41\x80\xBF"_sig,
#endif
        &MinecraftGame::_clientUpdate,
        sapphire::deRefCall>;
    return (this->*Hook::origin)();
}
