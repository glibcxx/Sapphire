#include "MinecraftGame.h"

MinecraftGame *MinecraftGame::ctor() {
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        "\x48\x89\x5C\x24\x00\x48\x89\x4C\x24\x00\x55\x56\x57\x48\x83\xEC\x00\x48\x8B\xF9\x33\xF6"_sig,
#elif MC_VERSION == v1_21_50
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x00\x48\x8B\xF9\x48\x89\x4C\x24\x00\x33\xED"_sig,
#elif MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x48\x8B\xF9\x48\x89\x4C\x24\x00\x33\xED"_sig,
#endif
        &MinecraftGame::ctor>;
    return (this->*Bind::origin)();
}

void MinecraftGame::initialize(int argc, char **argv) {
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2 || MC_VERSION == v1_21_50
        "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x55\x57\x41\x54\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4C\x8B\xF1\x45\x33\xE4\x48\x8D\x91"_sig,
#elif MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4C\x8B\xF1\x45\x33\xE4\x48\x8D\x91"_sig,
#endif
        &MinecraftGame::initialize,
        SPHR_FUNCDNAME>;
    return (this->*Bind::origin)(argc, argv);
}

void MinecraftGame::update() {
    using Bind = sapphire::bind::Fn<
        "\x40\x53\x48\x83\xEC\x00\x48\x8B\xD9\xE8\x00\x00\x00\x00\x48\x8B\x8B\x00\x00\x00\x00\xBA"_sig,
        &MinecraftGame::update,
        SPHR_FUNCDNAME>;
    return (this->*Bind::origin)();
}

void MinecraftGame::requestLeaveGame(bool switchScreen, bool sync) {
    using Bind = sapphire::bind::Fn<
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x48\x83\xEC\x00\x48\x8B\x41\x00\x48\x8B\xF9"_sig,
        &MinecraftGame::requestLeaveGame,
        SPHR_FUNCDNAME>;
    return (this->*Bind::origin)(switchScreen, sync);
}

void MinecraftGame::startFrame() {
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x80\xBE\x00\x00\x00\x00\x00\x74\x00\x48\x8B\x9E"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x41\x80\xBF\x00\x00\x00\x00\x00\x74\x00\x49\x8B\x9F"_sig,
#endif
        &MinecraftGame::startFrame>;
    return (this->*Bind::origin)();
}

void MinecraftGame::endFrame() {
    using Bind = sapphire::bind::Fn<
        [](uintptr_t addr) { return memory::deRef(addr + 2, memory::AsmOperation::CALL); }
            | "\x8B\x00\xE8\x00\x00\x00\x00\x00\x8B\x00\x00\x00\x00\x00\xC6\x80\xB8\x00\x00\x00\x00\x00\x8B"_sig,
        &MinecraftGame::endFrame>;
    return (this->*Bind::origin)();
}

void MinecraftGame::updateGraphics(IClientInstance &client, const Timer &timer) {
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\x46\x00\x48\x8D\x95"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x49\x8D\x4F\x00\x49\x8B\x47\x00\x48\x8D\x95\x00\x00\x00\x00\x48\x8B\x40\x00\xFF\x15\x00\x00\x00\x00\x90\x4D\x8B\xC4"_sig,
#endif
        &MinecraftGame::updateGraphics>;
    return (this->*Bind::origin)(client, timer);
}

void MinecraftGame::updateAudio(IClientInstance &client, const Timer &timer) {
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x0F\xB6\x44\x24\x00\x88\x86"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x0F\xB6\x44\x24\x00\x41\x88\x87"_sig,
#endif
        &MinecraftGame::updateAudio>;
    return (this->*Bind::origin)(client, timer);
}

void MinecraftGame::_update() {
    using Bind = sapphire::bind::Fn<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\x8B\x00\x00\x00\x00\xBA\x00\x00\x00\x00\x48\x83\xC4"_sig,
        &MinecraftGame::_update>;
    return (this->*Bind::origin)();
}

bool MinecraftGame::_clientUpdate() {
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x44\x0F\xB6\xF0\x88\x44\x24\x00\x80\xBE"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x0F\xB6\xF8\x88\x44\x24\x00\x41\x80\xBF"_sig,
#endif
        &MinecraftGame::_clientUpdate>;
    return (this->*Bind::origin)();
}

void MinecraftGame::_InitComplete() {
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        [](uintptr_t addr) { return memory::deRef(addr + 3, memory::AsmOperation::CALL); }
            | "\x48\x8B\xCE\xE8\x00\x00\x00\x00\x48\x8B\x8E\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x49\x8B\x16"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x49\x8B\xBE\x00\x00\x00\x00\x48\x8B\x07\x48\x85\xC0"_sig,
#endif
        &MinecraftGame::_InitComplete>;
    return (this->*Bind::origin)();
}

SerialWorkList::WorkResult MinecraftGame::_initFinish(std::shared_ptr<MinecraftGame::InitContext> &initContext) {
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x4C\x8B\xF2\x48\x8B\xF1\x45\x33\xFF"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x4C\x8B\xE2\x4C\x8B\xF1\x45\x33\xED"_sig,
#endif
        &MinecraftGame::_initFinish>;
    return (this->*Bind::origin)(initContext);
}