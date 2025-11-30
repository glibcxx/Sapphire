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

            SAPPHIRE_API("1.21.2", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\xB4\x24\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x49\x8B\xF0\x4C\x8B\xE2\x45\x33\xED")
            SAPPHIRE_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4D\x8B\xF8\x4C\x8B\xEA\x33\xFF")
            SDK_API void _insert(
                const EntityCreationContext &entityContext,
                const RenderSkyDescription  &description
            );

            SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\xB8\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x2B\xE0\x0F\x29\xB4\x24\x00\x00\x00\x00\x0F\x29\xBC\x24\x00\x00\x00\x00\x44\x0F\x29\x84\x24\x00\x00\x00\x00\x44\x0F\x29\x8C\x24\x00\x00\x00\x00\x44\x0F\x29\x94\x24\x00\x00\x00\x00\x44\x0F\x29\x9C\x24\x00\x00\x00\x00\x44\x0F\x29\xA4\x24\x00\x00\x00\x00\x44\x0F\x29\xAC\x24\x00\x00\x00\x00\x44\x0F\x29\xB4\x24\x00\x00\x00\x00\x44\x0F\x29\xBC\x24\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4D\x8B\xF0\x48\x8B\xFA")
            SDK_API void _insert(
                const EntityCreationContext       &entityContext,
                const RenderUIGamefaceDescription &description
            );

            SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "\x48\x8B\xC4\x48\x89\x58\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x44\x0F\x29\x40\x00\x44\x0F\x29\x48\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4D\x8B\xE8\x4C\x8B\xE2")
            SDK_API void _insert(
                const EntityCreationContext          &entityContext,
                const BlitFlipbookTextureDescription &descriptions
            );

            SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "\x48\x89\x5C\x24\x20\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\xFF\xFF\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x00\x8B\x00\x48\x89\x54\x24\x00\x48\x89\x4C\x24\x48")
            SDK_API void _insert(
                const EntityCreationContext         &entityContext,
                const RenderMeshFallbackDescription &descriptions
            );
        };

    } // namespace bgfxbridge

} // namespace mce::framebuilder