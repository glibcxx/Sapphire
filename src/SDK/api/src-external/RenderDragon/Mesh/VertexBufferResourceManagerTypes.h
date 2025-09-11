#pragma once

#include "SDK/api/src-external/bgfx/bgfx.h"

namespace dragon {

    // size: 8
    struct VertexBufferView {
        VertexBufferView() = delete;

        // vtb+0
        virtual ~VertexBufferView();
    };

    // size: 112 (1.21.50)
    struct ResolvedVertexBuffer {
        std::variant<bgfx::VertexBufferHandle, bgfx::DynamicVertexBufferHandle> mResource;         // off+0
        uint32_t                                                                mOffset;           // off+4
        uint32_t                                                                mCount;            // off+8
        bgfx::VertexDeclHandle                                                  mVertexDeclHandle; // off+12
        bgfx::VertexDecl                                                        mVertexDecl;       // off+16
        std::unique_ptr<dragon::VertexBufferView>                               mStorageView;      // off+104

        ~ResolvedVertexBuffer() = default;
    };

    // size: 112 (1.21.50)
    struct ResolvedVertexBufferResource : ResolvedVertexBuffer {};

} // namespace dragon