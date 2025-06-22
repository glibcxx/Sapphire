#pragma once

#include <string>
#include "SDK/api/src-deps/Coregraphics/BufferDescription.h"

namespace dragon {

    class TextureDescription : public cg::BufferDescription {
        std::string mIdentifier; // off+72
        // ...
    };

} // namespace dragon
