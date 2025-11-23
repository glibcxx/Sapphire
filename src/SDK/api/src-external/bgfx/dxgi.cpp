#include "dxgi.h"

namespace bgfx {

    bool Dxgi::init(bgfx::Caps &_caps) {
        using Bind = sapphire::bind::Fn<
            sapphire::deRefLea | "\xE8\x00\x00\x00\x00\x84\xC0\x75\x00\x41\xBD"_sig, // 1.21.50
            &Dxgi::init>;
        return (this->*Bind::origin)(_caps);
    }

} // namespace bgfx