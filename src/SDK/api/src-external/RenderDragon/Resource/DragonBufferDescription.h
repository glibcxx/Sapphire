#pragma once

#include <string>
#include "SDK/api/src-deps/Coregraphics/BufferDescription.h"

namespace dragon {

    // size: 32
    struct BufferDescription : public cg::BufferDescription {
        std::string mDebugName; // off+8
    };

} // namespace dragon