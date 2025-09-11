#pragma once

#include "SDK/api/src-deps/Core/CheckedResourceService/TransactionContainer.h"
#include "../Mesh/VertexFormat.h"
#include "DragonBufferResourceService.h"

namespace dragon {

    // size: 248 (1.21.50)
    struct CreateVertexBufferTransaction {
        using TransactionContainer = mce::TransactionContainer<
            dragon::CreateVertexBufferTransaction,
            dragon::VertexBufferResourceService>;

        std::vector<unsigned char> mBufferData;   // off+0
        uint32_t                   mBufferStride; // off+24
        uint32_t                   mCount;        // off+28
        std::string                mDebugName;    // off+32
        dragon::mesh::VertexFormat mVertexFormat; // off+64
    };
} // namespace dragon