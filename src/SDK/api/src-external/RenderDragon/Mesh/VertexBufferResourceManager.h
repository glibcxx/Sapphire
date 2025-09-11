#pragma once

#include <optional>
#include <variant>

#include "SDK/core/ApiManager.h"
#include "SDK/api/src-external/RenderDragon/Rendering/BufferResourceManager.h"
#include "SDK/api/src-external/RenderDragon/Memory/BufferSource.h"
#include "VertexBufferResourceManagerTypes.h"
#include "VertexFormat.h"

namespace dragon::mesh {

    class VertexDeclManager;

    namespace details {

        struct VertexBufferHandleTypeKey {};

    } // namespace details

    // size: 224
    struct VertexBufferDescription {
        dragon::mesh::VertexFormat mFormat;       // off+0
        uint64_t                   mStride;       // off+184
        std::optional<uint16_t>    mFormatHandle; // off+192
        uint64_t                   mDataSize;     // off+200
        uint64_t                   mVertexCount;  // off+208
        uint16_t                   mFlags;        // off+216
    };

    inline namespace _anonymous_namespace_at_VertexBufferResourceManager_cpp_ {

        struct VertexBufferType {
            using HandleTypeKey = details::VertexBufferHandleTypeKey;
            using ResourceType = std::variant<bgfx::VertexBufferHandle, bgfx::DynamicVertexBufferHandle>;
            using ResolvedResourceType = std::optional<ResolvedVertexBuffer>;
            using ResourcePublicDescription = VertexBufferDescription;

            // size: 24
            struct ResourceDescription {
                bgfx::VertexDeclHandle mDeclHandle; // off+0
                uint64_t               mSize;       // off+8
                uint64_t               mStride;     // off+16
            };

            // size: 360 (1.21.50)
            struct ResourceCreationParameters {
                bool                                        mIsDynamic;        // off+0
                dragon::mesh::VertexFormat                  mFormat;           // off+8
                std::optional<uint16_t>                     mFormatHandle;     // off+192
                bgfx::VertexDeclHandle                      mVertexDeclHandle; // off+196
                bgfx::VertexDecl                            mVertexDecl;       // off+200
                uint64_t                                    mSizeInBytes;      // off+288
                std::optional<dragon::memory::BufferSource> mInitialData;      // off+296
                uint16_t                                    mFlags;            // off+352
            };

            // size: 56
            struct ItemResourceUpdateParameters {
                uint32_t                     mStartIndex; // off+0
                dragon::memory::BufferSource mUpdateData; // off+8
            };

            // size: 56
            struct OffsetResourceUpdateParameters {
                uint32_t                     mOffset;     // off+0
                uint16_t                     mStride;     // off+4
                dragon::memory::BufferSource mUpdateData; // off+8
            };
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