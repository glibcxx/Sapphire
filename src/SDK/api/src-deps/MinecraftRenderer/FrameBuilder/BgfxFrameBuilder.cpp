#include "BgfxFrameBuilder.h"

namespace mce::framebuilder {

    void BgfxFrameBuilder::lambda_at_mce__framebuilder__BgfxFrameBuilder__endFrame::operator()() const {
        using Hook = sapphire::ApiLoader<
            "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x55\x57\x41\x54\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x48\x8B\xD9\x45\x33\xE4\x0F\x57\xC0"_sig,
            &BgfxFrameBuilder::lambda_at_mce__framebuilder__BgfxFrameBuilder__endFrame::operator()>;
        (this->*Hook::origin)();
    }

} // namespace mce::framebuilder
