#include "BgfxFrameExtractor.h"

void mce::framebuilder::bgfxbridge::BgfxFrameExtractor::_insert(
    const mce::framebuilder::bgfxbridge::EntityCreationContext &entityContext,
    const mce::framebuilder::RenderSkyDescription              &description
) {
    using Hook = core::ApiLoader<
        "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4D\x8B\xF8\x4C\x8B\xEA\x33\xFF"_sig,
        &mce::framebuilder::bgfxbridge::BgfxFrameExtractor::_insert>;
    (this->*Hook::origin)(entityContext, description);
}