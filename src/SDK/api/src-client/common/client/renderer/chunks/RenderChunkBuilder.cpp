#include "RenderChunkBuilder.h"

void RenderChunkBuilder::build(
    RenderChunkGeometry                                     &renderChunkGeometry,
    bool                                                     transparentLeaves,
    BakedBlockLightType                                      lightingType,
    bool                                                     forExport,
    const mce::framebuilder::FrameLightingModelCapabilities &lightingModelCapabilities
) {
    using Bind = sapphire::bind::Fn<
        sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x8B\x47\x00\x48\x8D\x54\x24\x00\x89\x03"_sig, // 1.21.50
        &RenderChunkBuilder::build>;
    (this->*Bind::origin)(renderChunkGeometry, transparentLeaves, lightingType, forExport, lightingModelCapabilities);
}