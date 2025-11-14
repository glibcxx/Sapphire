#pragma once

#include "SDK/api/src-external/RenderDragon/Rendering/ClipSpaceOrigin.h"
#include "SDK/api/src-external/RenderDragon/Task/GraphicsTasks.h"
#include "SDK/api/src-external/RenderDragon/Rendering/BgfxPrimitiveIndexBuffer.h"
#include "SDK/api/src-external/RenderDragon/Rendering/LightingModels.h"
#include "SDK/api/src-external/RenderDragon/Materials/MaterialResourceManager.h"
#include "SDK/api/src-external/RenderDragon/FrameObject/Frame.h"
#include "SDK/api/src-deps/MinecraftRenderer/Resources/TextureResourceService.h"

namespace mce::framebuilder {

    struct RenderSkyDescription;
    struct RenderMeshFallbackDescription;
    struct BlitFlipbookTextureDescription;
    struct RenderUIGamefaceDescription;

    namespace bgfxbridge {

        // size: 120 (1.21.50)
        struct EntityCreationContext {
            Core::CpuRingBufferAllocator<uint8_t>        &mAllocator;               // off+0
            dragon::rendering::ClipSpaceOrigin            clipSaceOrigin;           // off+8
            dragon::frameobject::GameFrame               &mFrame;                   // off+16
            dragon::task::GraphicsTasks                  &mGraphicsTasks;           // off+24
            dragon::task::AsyncTasksScope                &mAsyncTasks;              // off+32
            dragon::VertexBufferResourceService          &mVbResourceService;       // off+40
            dragon::IndexBufferResourceService           &mIbResourceService;       // off+48
            dragon::AccelerationStructureResourceService &mAsResourceService;       // off+56
            dragon::MaterialResourceService              &mMaterialResourceService; // off+64
            dragon::TextureResourceService               &mTextureResourceService;  // off+72
            dragon::mesh::VertexDeclManager              &mVertexDeclManager;       // off+80
            dragon::rendering::BgfxPrimitiveIndexBuffer  &mQuadIndexBuffer;         // off+88
            dragon::rendering::BgfxPrimitiveIndexBuffer  &mSequenceIndexBuffer;     // off+96
            dragon::materials::MaterialResourceManager   &mMaterialsManager;        // off+104
            dragon::rendering::LightingModels             mLightingModels;          // off+112
            uint8_t                                       mUnk116;                  // off+116
        };

        // size: 1152 (1.21.50)
        class BgfxFrameExtractor {
        public:
            // std::unique_ptr<dragon::materials::MaterialResourceManager> mMaterialsManager; // off+1008

            BgfxFrameExtractor *ctor(
                uintptr_t a1,
                uintptr_t a2,
                uintptr_t a3,
                uintptr_t a4,
                uintptr_t a5,
                uintptr_t a6
            ); // \xE8\x00\x00\x00\x00\xEB\x00\x49\x8B\xC4\x48\x8B\x5D\x00\x48\x89\x45 1.21.50

            SDK_API void _insert(
                const EntityCreationContext &entityContext,
                const RenderSkyDescription  &description
            );

            SDK_API void _insert(
                const EntityCreationContext       &entityContext,
                const RenderUIGamefaceDescription &description
            );

            SDK_API void _insert(
                const EntityCreationContext          &entityContext,
                const BlitFlipbookTextureDescription &descriptions
            );

            SDK_API void _insert(
                const EntityCreationContext         &entityContext,
                const RenderMeshFallbackDescription &descriptions
            );
        };

    } // namespace bgfxbridge

} // namespace mce::framebuilder