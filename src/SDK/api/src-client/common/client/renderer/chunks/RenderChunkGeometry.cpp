#include "RenderChunkGeometry.h"

void RenderChunkGeometry::endRebuild(
    RenderChunkBuilder           &builder,
    mce::BufferResourceService   &bufferResourceService,
    bool                          isBuilding,
    const dragon::RenderMetadata &renderMetadata,
    bool                          enforceInterlocks
) {
    using Hook = sapphire::ApiLoader<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xB9\x00\x00\x00\x00\x33\xC0\xF0\x0F\xB1\x4B"_sig, // 1.21.50
        &RenderChunkGeometry::endRebuild>;
    (this->*Hook::origin)(builder, bufferResourceService, isBuilding, renderMetadata, enforceInterlocks);
}