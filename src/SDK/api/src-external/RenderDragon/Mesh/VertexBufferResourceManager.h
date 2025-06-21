#pragma once

#include <optional>
#include <variant>

#include "SDK/core/Core.h"
#include "SDK/api/src-external/RenderDragon/Rendering/BufferResourceManager.h"
#include "VertexBufferResourceManagerTypes.h"

namespace dragon::mesh {

    class VertexDeclManager;

    namespace details {

        struct VertexBufferHandleTypeKey {};

    } // namespace details

    struct VertexBufferDescription {
        // ...
    };

    inline namespace _anonymous_namespace_at_VertexBufferResourceManager_cpp_ {

        struct VertexBufferType {
            using HandleTypeKey = details::VertexBufferHandleTypeKey;
            // using ResourceType = std::variant<bgfx::VertexBufferHandle, bgfx::DynamicVertexBufferHandle>;
            using ResolvedResourceType = std::optional<ResolvedVertexBuffer>;
            using ResourcePublicDescription = VertexBufferDescription;
        };

    } // namespace _anonymous_namespace_at_VertexBufferResourceManager_cpp_

    // size: 16
    class VertexBufferResourceManager {
    public:
        // size: 704
        class Impl : public dragon::rendering::BufferResourceManager<VertexBufferType> {};

        std::unique_ptr<Impl> mImpl;              // off+0
        VertexDeclManager    &mVertexDeclManager; // off+8
    };
    static_assert(sizeof(VertexBufferResourceManager::Impl) == 704);

} // namespace dragon::mesh