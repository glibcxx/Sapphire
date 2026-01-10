#pragma once

#include "VertexFormat.h"

namespace mce {

    // https://github.com/LiteLDev/LeviLamina/blob/dad11c69b20f644ab7192d45add4e0be1d54d432/src/mc/deps/renderer/VertexLayout.h#L7

    // size: 70
    class VertexLayout {
        VertexFormat mVertexFormat[2];
        bool         mUnk68; // off+68
    };

} // namespace mce