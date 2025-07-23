#pragma once

#include <optional>
#include <variant>

#include "SDK/core/ApiManager.h"
#include "SDK/api/src-external/RenderDragon/Rendering/BufferResourceManager.h"
#include "IndexBufferResourceManagerTypes.h"

namespace dragon::mesh {

    namespace details {

        struct IndexBufferHandleTypeKey {};

    } // namespace details

    struct IndexBufferDescription {
        // ...
    };

    inline namespace _anonymous_namespace_at_IndexBufferResourceManager_cpp_ {

        struct IndexBufferType {
            using HandleTypeKey = details::IndexBufferHandleTypeKey;
            // using ResourceType = std::variant<bgfx::IndexBufferHandle, bgfx::DynamicIndexBufferHandle>;
            using ResolvedResourceType = std::optional<ResolvedIndexBuffer>;
            using ResourcePublicDescription = IndexBufferDescription;
        };

    } // namespace _anonymous_namespace_at_IndexBufferResourceManager_cpp_

    // size: 8
    class IndexBufferResourceManager {
    public:
        // size: 704
        class Impl : public dragon::rendering::BufferResourceManager<IndexBufferType> {};

        std::unique_ptr<Impl> mImpl; // off+0
    };
    static_assert(sizeof(IndexBufferResourceManager::Impl) == 704);

} // namespace dragon::mesh