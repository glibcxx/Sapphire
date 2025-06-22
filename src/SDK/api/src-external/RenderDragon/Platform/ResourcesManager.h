#pragma once

#include "SDK/api/src-external/RenderDragon/Rendering/TextureResourceManager.h"
#include "SDK/api/src-external/RenderDragon/Mesh/VertexDeclManager.h"
#include "SDK/api/src-external/RenderDragon/Mesh/VertexBufferResourceManager.h"
#include "SDK/api/src-external/RenderDragon/Mesh/IndexBufferResourceManager.h"
#include "SDK/api/src-external/RenderDragon/Mesh/SpanRegistry.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonBufferResourceService.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonTextureResourceService.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonMaterialResourceService.h"

namespace dragon::platform {

    // size: 16
    struct ResourcesManagerConfiguration {
        std::optional<uint32_t> mVertexBufferCombinedAllocationPageSize; // off+0
        std::optional<uint32_t> mIndexBufferCombinedAllocationPageSize;  // off+8
    };

    // size: 1832 (1.21.50)
    class ResourcesManager {
    public:
        dragon::rendering::TextureResourceManager    mTextureManager;                       // off+0
        dragon::mesh::VertexDeclManager              mVertexDeclManager;                    // off+32
        dragon::mesh::VertexBufferResourceManager    mVertexBufferManager;                  // off+40
        dragon::mesh::IndexBufferResourceManager     mIndexBufferManager;                   // off+56
        dragon::mesh::VertexSpanRegistry             mVertexSpanStorage;                    // off+64
        dragon::mesh::IndexSpanRegistry              mIndexSpanStorage;                     // off+512
        dragon::materials::CompiledMaterialManager   mCompiledMaterialManager;              // off+776
        dragon::DragonBufferResourceServiceContext   mBufferResourceServiceContext;         // off+1152
        dragon::VertexBufferResourceService          mVertexBufferResourceService;          // off+1176
        dragon::IndexBufferResourceService           mIndexBufferResourceService;           // off+1256
        dragon::ShaderBufferResourceService          mShaderBufferResourceService;          // off+1336
        dragon::MaterialResourceServiceContext       mMaterialResourceServiceContext;       // off+1416
        dragon::MaterialResourceService              mMaterialResourceService;              // off+1488
        dragon::TextureResourceService               mTextureResourceService;               // off+1616
        dragon::AccelerationStructureResourceService mAccelerationStructureResourceService; // off+1688

        ResourcesManager *ctor(
            dragon::task::GraphicsTasks                                    &graphicsTasks,
            const ResourcesManagerConfiguration                            &configuration,
            const dragon::materials::CompiledMaterialManager::Unk144B99FE0 &a3,
            const std::function<uintptr_t(uintptr_t)>                      &a4
        );
    };
    const auto a = offsetof(ResourcesManager, mAccelerationStructureResourceService);

} // namespace dragon::platform