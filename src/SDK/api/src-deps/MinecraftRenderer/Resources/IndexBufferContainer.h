#pragma once

#include <string_view>
#include <variant>

#include "SDK/core/ApiManager.h"
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

        SDK_API void loadIndexBuffer(
            mce::BufferResourceService &bufferResourceService,
            const uint32_t              stride,
            const char *const           data,
            const uint32_t              count,
            std::string_view            debugName
        );
        MARK_HOOKABLE(&IndexBufferContainer::loadIndexBuffer)
    };

} // namespace mce