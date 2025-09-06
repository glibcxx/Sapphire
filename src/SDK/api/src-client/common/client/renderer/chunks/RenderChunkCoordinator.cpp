#include "RenderChunkCoordinator.h"

RenderChunkCoordinator *RenderChunkCoordinator::ctor(
    Level                      &level,
    LevelRenderer              &levelRenderer,
    AutomaticID<Dimension, int> dimensionID
) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xEB\x00\x49\x8B\xF6\x0F\x57\xC0\x0F\x11\x44\x24"_sig, // 1.21.50
        &RenderChunkCoordinator::ctor>;
    return (this->*Hook::origin)(level, levelRenderer, dimensionID);
}

void RenderChunkCoordinator::tick() {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\x03\x48\x8B\xD8\x48\x3B\xC6\x75\x00\x48\x8B\x8F"_sig, // 1.21.50
        &RenderChunkCoordinator::tick>;
    (this->*Hook::origin)();
}

std::shared_ptr<RenderChunkShared> RenderChunkCoordinator::getOrCreateChunkAtPos(const SubChunkPos &pos) {
    using Hook = sapphire::ApiLoader<
        "\x40\x53\x55\x56\x57\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x4D\x8B\xF8"_sig, // 1.21.50
        &RenderChunkCoordinator::getOrCreateChunkAtPos>;
    return (this->*Hook::origin)(pos);
}

void RenderChunkCoordinator::_handleVisibilityUpdates() {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\x03\x48\x8B\xD8\x48\x3B\xC7\x75\x00\x4D\x8B\xC5"_sig, // 1.21.50
        &RenderChunkCoordinator::_handleVisibilityUpdates>;
    (this->*Hook::origin)();
}