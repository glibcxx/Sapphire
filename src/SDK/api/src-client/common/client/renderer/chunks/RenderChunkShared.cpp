#include "RenderChunkShared.h"
#include "SDK/core/SymbolResolver.h"

RenderChunkShared *RenderChunkShared::ctor(const BlockPos &p) {
    using Bind = sapphire::bind::Fn<
        "\xE8\x00\x00\x00\x00\xEB\x00\x48\x8B\xFD\xC7\x44\x24"_sig, // 1.21.50
        &RenderChunkShared::ctor>;
    return (this->*Bind::origin)(p);
}

void RenderChunkShared::startRebuild(RenderChunkBuilder &builder, const Vec3 &currentCameraPosition) {
    using Bind = sapphire::bind::Fn<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x49\x8B\x17\x8B\x42"_sig, // 1.21.50
        &RenderChunkShared::startRebuild>;
    return (this->*Bind::origin)(builder, currentCameraPosition);
}

void RenderChunkShared::rebuild(
    RenderChunkBuilder                                      &builder,
    bool                                                     transparentLeaves,
    BakedBlockLightType                                      lightingType,
    bool                                                     forExport,
    const mce::framebuilder::FrameLightingModelCapabilities &lightingModelCapabilities
) {
    using Bind = sapphire::bind::Fn<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\x5F\x00\x48\x8B\x4B"_sig, // 1.21.50
        &RenderChunkShared::rebuild>;
    return (this->*Bind::origin)(builder, transparentLeaves, lightingType, forExport, lightingModelCapabilities);
}

void RenderChunkShared::addEntityBlockSyncMessages(buffer_span<ActorBlockSyncMessage> syncMsgList) {
    using Bind = sapphire::bind::Fn<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\xD7\x49\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x4C\x89\x65"_sig, // 1.21.50
        &RenderChunkShared::addEntityBlockSyncMessages>;
    (this->*Bind::origin)(std::move(syncMsgList));
}

void RenderChunkShared::addBlockEntityBlockSyncMessages(
    buffer_span<BlockActorBlockSyncMessage> syncMsgList
) {
    using Bind = sapphire::bind::Fn<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\xD7\x49\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\x45"_sig, // 1.21.50
        &RenderChunkShared::addBlockEntityBlockSyncMessages>;
    (this->*Bind::origin)(std::move(syncMsgList));
}