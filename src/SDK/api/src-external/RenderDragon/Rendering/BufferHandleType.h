#pragma once

#include <cstdint>

namespace dragon::rendering::details {

    struct TextureHandleTypeKey {};

    template <typename HandleTypeKey, typename Val = uint64_t>
    struct BufferHandleTypeWrapper {
        Val mValue; // off+0

        operator Val() const {
            return mValue;
        }
    };

} // namespace dragon::rendering::details