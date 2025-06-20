#pragma once

#include "SDK/api/src-external/RenderDragon/Rendering/ClipSpaceOrigin.h"
#include "SDK/api/src-external/RenderDragon/Rendering/GraphicsTasks.h"
#include "SDK/api/src-external/RenderDragon/Mesh/VertexDeclManager.h"
#include "SDK/api/src-external/RenderDragon/Rendering/BgfxPrimitiveIndexBuffer.h"
#include "SDK/api/src-external/RenderDragon/Materials/MaterialResourceManager.h"

namespace mce::framebuilder {

    struct RenderSkyDescription;

    namespace bgfxbridge {

        // size: 120 (1.21.50)
        struct EntityCreationContext {
            uintptr_t                                   &mAllocator;           // off+0
            dragon::rendering::ClipSpaceOrigin           clipSaceOrigin;       // off+8
            uintptr_t                                   &mFrame;               // off+16
            dragon::rendering::GraphicsTasks            &mGraphicsTasks;       // off+24
            dragon::rendering::AsyncTasksScope          &mAsyncTasks;          // off+32
            uintptr_t                                   &mVbResourceService;   // off+40
            uintptr_t                                   &mIbResourceService;   // off+48
            uintptr_t                                   &mAsResourceService;   // off+56
            uintptr_t                                   &mUnk64;               // off+64
            uintptr_t                                   &mUnk72;               // off+72
            dragon::mesh::VertexDeclManager             &mVertexDeclManager;   // off+80
            dragon::rendering::BgfxPrimitiveIndexBuffer &mQuadIndexBuffer;     // off+88
            dragon::rendering::BgfxPrimitiveIndexBuffer &mSequenceIndexBuffer; // off+96
            dragon::materials::MaterialResourceManager  &mMaterialsManager;    // off+104
            uint32_t                                     mUnk112;              // off+112
            uint8_t                                      mUnk116;              // off+116
        };

        class BgfxFrameExtractor {
        public:
            void _insert(
                const EntityCreationContext &entityContext,
                const RenderSkyDescription  &description
            );
        };

    } // namespace bgfxbridge

} // namespace mce::framebuilder