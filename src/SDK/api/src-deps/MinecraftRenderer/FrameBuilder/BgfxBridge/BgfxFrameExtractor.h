#pragma once

#include "SDK/api/src-external/RenderDragon/Rendering/ClipSpaceOrigin.h"
#include "SDK/api/src-external/RenderDragon/Task/GraphicsTasks.h"
#include "SDK/api/src-external/RenderDragon/Rendering/BgfxPrimitiveIndexBuffer.h"
#include "SDK/api/src-external/RenderDragon/Materials/MaterialResourceManager.h"
#include "SDK/api/src-external/RenderDragon/FrameObject/Frame.h"
#include "src/SDK/api/src-deps/MinecraftRenderer/Resources/TextureResourceService.h"

namespace mce::framebuilder {

    struct RenderSkyDescription;
    struct BlitFlipbookSingleTextureDescription;

    struct BlitFlipbookTextureDescription {
        const dragon::ServerTexture                           mRenderTargetTexture; // off+0
        gsl::span<const BlitFlipbookSingleTextureDescription> mTextures;            // off+24
    };

    struct GamefaceRenderTarget {
        std::variant<std::monostate, dragon::ClientTexture, mce::ClientTexture> mColorTargetTextureHandle; // off+0
        std::variant<std::monostate, dragon::ClientTexture, mce::ClientTexture> mDepthTargetTextureHandle; // off+32
        glm::tvec4<int>                                                         mViewport;                 // off+64
        glm::tvec4<int>                                                         mScissor;                  // off+80
        bool                                                                    scissorEnabled;            // off+96
    };

    struct GamefacePipelineState {
        enum class Shader : int {
            Standard = 0,
            StandardRare = 1,
            Stencil = 2,
            StencilRare = 3,
            StenciPath = 4,
            BatchedStandard = 5,
            ClearQuad = 6,
            Renoir = 7,
            Path = 8,
            TexturesWithColorMix = 9,
        };

        Shader               shader;       // off+0
        GamefaceRenderTarget renderTarget; // off+8
    };

    struct RenderUIGamefaceDescription {
        const GamefacePipelineState mPipelineState;
    };

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
            uint32_t                                      mUnk112;                  // off+112
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
        };

    } // namespace bgfxbridge

} // namespace mce::framebuilder