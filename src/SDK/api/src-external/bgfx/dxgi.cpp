#include "dxgi.h"

namespace bgfx {

    bool Dxgi::init(bgfx::Caps &_caps) {
        using Hook = sapphire::ApiLoader<
            sapphire::deRefLea | "\xE8\x00\x00\x00\x00\x84\xC0\x75\x00\x41\xBD"_sig, // 1.21.50
            &Dxgi::init>;
        return (this->*Hook::origin)(_caps);
    }

} // namespace bgfx