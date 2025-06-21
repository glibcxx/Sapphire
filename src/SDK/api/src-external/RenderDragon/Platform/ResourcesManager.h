#pragma once

#include "SDK/api/src-external/RenderDragon/Rendering/TextureResourceManager.h"
#include "SDK/api/src-external/RenderDragon/Mesh/VertexDeclManager.h"
#include "SDK/api/src-external/RenderDragon/Mesh/VertexBufferResourceManager.h"
#include "SDK/api/src-external/RenderDragon/Mesh/IndexBufferResourceManager.h"

namespace dragon::platform {

    // size: 16
    struct ResourcesManagerConfiguration {
        std::optional<uint32_t> mVertexBufferCombinedAllocationPageSize; // off+0
        std::optional<uint32_t> mIndexBufferCombinedAllocationPageSize;  // off+8
    };

    // size: 1832 (1.21.50)
    class ResourcesManager {
    public:
        dragon::rendering::TextureResourceManager mTextureManager;      // off+0
        dragon::mesh::VertexDeclManager           mVertexDeclManager;   // off+32
        dragon::mesh::VertexBufferResourceManager mVertexBufferManager; // off+40
        dragon::mesh::IndexBufferResourceManager  mIndexBufferManager;  // off+56
        // dragon::mesh::VertexSpanRegistry          mVertexSpanStorage;   // off+64
        // dragon::mesh::IndexSpanRegistry           mIndexSpanStorage;    // off+512

        ResourcesManager *ctor(uintptr_t a1, uintptr_t a2, uintptr_t a3, uintptr_t a4);
    };

} // namespace dragon::platform