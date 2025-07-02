#pragma once

#include "SDK/core/Core.h"

#include <variant>
#include "SDK/api/src-deps/Core/CheckedResourceService/ServerResourcePointer.h"

namespace DFC /*dragon::frameobject::components*/ {

    // size: 152 (1.21.50)
    struct MeshFilter {
        uint64_t mVertexBuffer[4]; // off+0, std::variant<...>
        uint32_t mVertexOffset;    // off+32
        uint64_t mVertexCount;     // off+40
        uint8_t  mUnk48;           // off+48
        uint64_t mIndexBuffer[4];  // off+56, std::variant<...>
        uint32_t mIndexOffset;     // off+88
        uint64_t mUnkBuffer[4];    // off+96, std::variant<...>
        uint32_t mUnkOffset;       // off+128
        uint64_t mUnkCount;        // off+136
        uint64_t mAllocator;       // off+144

        MeshFilter(MeshFilter &&rhs) {
            memcpy(this, &rhs, sizeof(MeshFilter));
            memset(&rhs, 0, sizeof(MeshFilter));
        }

        ~MeshFilter() {
            this->dtor();
        }

        SDK_API void dtor() noexcept;
    };

} // namespace DFC
