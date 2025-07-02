#include "BgfxFrameExtractor.h"

namespace mce::framebuilder::bgfxbridge {

    void BgfxFrameExtractor::_insert(
        const EntityCreationContext &entityContext,
        const RenderSkyDescription  &description
    ) {
        using Fn = void (BgfxFrameExtractor::*)(const EntityCreationContext &, const RenderSkyDescription &);
        using Hook = core::ApiLoader<
            "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4D\x8B\xF8\x4C\x8B\xEA\x33\xFF"_sig,
            (Fn)&BgfxFrameExtractor::_insert>;
        (this->*Hook::origin)(entityContext, description);
    }

    void BgfxFrameExtractor::_insert(const EntityCreationContext &entityContext, const RenderUIGamefaceDescription &description) {
        using Fn = void (BgfxFrameExtractor::*)(const EntityCreationContext &, const RenderUIGamefaceDescription &);
        using Hook = core::ApiLoader<
            "\xE8\x00\x00\x00\x00\x4C\x8D\x9C\x24\x00\x00\x00\x00\x49\x8B\x5B\x00\x49\x8B\x73\x00\x49\x8B\xE3\x5F\xC3\x4C\x8B\x02"_sig,
            (Fn)&BgfxFrameExtractor::_insert,
            core::deRefCall>;
        (this->*Hook::origin)(entityContext, description);
    }

    void BgfxFrameExtractor::_insert(const EntityCreationContext &entityContext, const BlitFlipbookTextureDescription &descriptions) {
        using Fn = void (BgfxFrameExtractor::*)(const EntityCreationContext &, const BlitFlipbookTextureDescription &);
        using Hook = core::ApiLoader<
            "\xE8\x00\x00\x00\x00\xEB\x00\x4C\x8B\x02\x48\x8B\x51\x00\x48\x8B\x09\xE8\x00\x00\x00\x00\xEB"_sig,
            (Fn)&BgfxFrameExtractor::_insert,
            core::deRefCall>;
        (this->*Hook::origin)(entityContext, descriptions);
    }

} // namespace mce::framebuilder::bgfxbridge