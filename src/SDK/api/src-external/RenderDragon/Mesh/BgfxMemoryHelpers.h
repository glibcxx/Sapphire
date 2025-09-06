#pragma once

#include <memory>
#include "macros/Macros.h"

namespace dragon::memory {

inline namespace _anonymous_namespace_at_BgfxMemoryHelpers_cpp_ {

    template <typename T>
    struct BgfxDataWrapper {
        const std::shared_ptr<T> mData; // off+0

        static void _bgfxDeleter(void *, void *userData) {
            delete static_cast<std::shared_ptr<T> *>(userData);
        }
    };

}

} // namespace dragon::memory::_anonymous_namespace_at_BgfxMemoryHelpers_cpp_