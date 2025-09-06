#include "RenderChunkShared.h"
#include "SDK/core/ApiManager.h"

RenderChunkShared *RenderChunkShared::ctor(const BlockPos &p) {
    using Hook = sapphire::ApiLoader<
        "\xE8\x00\x00\x00\x00\xEB\x00\x48\x8B\xFD\xC7\x44\x24"_sig, // 1.21.50
        &RenderChunkShared::ctor>;
    return (this->*Hook::origin)(p);
}

void RenderChunkShared::startRebuild(RenderChunkBuilder &builder, const Vec3 &currentCameraPosition) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x49\x8B\x17\x8B\x42"_sig, // 1.21.50
        &RenderChunkShared::startRebuild>;
    return (this->*Hook::origin)(builder, currentCameraPosition);
}

void RenderChunkShared::rebuild(
    RenderChunkBuilder &builder,
    bool                transparentLeaves,
    BakedBlockLightType lightingType,
    bool                forExport,
    bool                usePBRFormat,
    bool                emitPointLights
) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\x5F\x00\x48\x8B\x4B"_sig, // 1.21.50
        &RenderChunkShared::rebuild>;
    return (this->*Hook::origin)(builder, transparentLeaves, lightingType, forExport, usePBRFormat, emitPointLights);
}

void RenderChunkShared::addEntityBlockSyncMessages(buffer_span<ActorBlockSyncMessage> syncMsgList) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\xD7\x49\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x4C\x89\x65"_sig, // 1.21.50
        &RenderChunkShared::addEntityBlockSyncMessages>;
    (this->*Hook::origin)(std::move(syncMsgList));
}

void RenderChunkShared::addBlockEntityBlockSyncMessages(
    buffer_span<BlockActorBlockSyncMessage> syncMsgList
) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\xD7\x49\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\x45"_sig, // 1.21.50
        &RenderChunkShared::addBlockEntityBlockSyncMessages>;
    (this->*Hook::origin)(std::move(syncMsgList));
}