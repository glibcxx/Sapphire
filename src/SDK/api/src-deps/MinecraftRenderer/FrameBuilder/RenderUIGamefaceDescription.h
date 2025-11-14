#pragma once

#include "SDK/api/src-deps/MinecraftRenderer/resources/TextureResourceService.h"

namespace mce::framebuilder {

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

} // namespace mce::framebuilder