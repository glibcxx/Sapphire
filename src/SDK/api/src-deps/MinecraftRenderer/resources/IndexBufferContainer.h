#pragma once

#include <string_view>
#include <variant>

#include "SDK/api/src-deps/Core/CheckedResourceService/ClientResourcePointer.h"
#include "SDK/api/src-external/RenderDragon/Mesh/VertexBufferResourceManagerTypes.h"
#include "SDK/api/src-external/RenderDragon/Mesh/IndexBufferResourceManagerTypes.h"

namespace mce {

    class Buffer {
         // todo
    };
    class BufferResourceService;

    // size: 40
    class IndexBufferContainer {
    public:
        mce::ClientResourcePointer<
            std::variant<
                std::monostate,
                mce::Buffer,
                mce::ClientResourcePointer<dragon::ResolvedVertexBufferResource>,
                mce::ClientResourcePointer<dragon::ResolvedIndexBufferResource>>>
                 mIndexBuffer; // off+0
        uint32_t mIndexCount;  // off+24
        uint32_t mIndexSize;   // off+28
        int32_t  mIteration;   // off+32

        SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x41\x8B\x00\x00\x89\x43\x20\x48\x8D")
        SDK_API void loadIndexBuffer(
            mce::BufferResourceService &bufferResourceService,
            const uint32_t              stride,
            const char *const           data,
            const uint32_t              count,
            std::string_view            debugName
        );
    };

} // namespace mce