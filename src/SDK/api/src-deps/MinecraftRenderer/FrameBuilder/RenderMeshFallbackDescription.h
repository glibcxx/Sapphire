#pragma once

#include "SDK/api/src-client/common/client/renderer/MaterialPtr.h"
#include "SDK/api/src-deps/Core/Math/Color.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonTextureResourceService.h"
#include "SDK/api/src-deps/MinecraftRenderer/framebuilder/RenderMetadata.h"
#include "FogDescription.h"

namespace mce {
    class Mesh;
    class GlobalConstantBuffers;
} // namespace mce

namespace mce::framebuilder {

    // size: 440 (1.21.2), 480 (1.21.50), 488 (1.21.60)
    struct RenderMeshFallbackDescription {
        ;                       // off+(1.21.2/1.21.50)
        const mce::Mesh &mMesh; // off+0

        struct static_vector_dragon__ServerTexture_8_ {
            SPHR_UNINIT(dragon::ServerTexture mData[8];)
            size_t mSize;
        };
        static_vector_dragon__ServerTexture_8_ mTextures;                    // off+8
        glm::tmat4x4<float>                    mWorldMatrix;                 // off+208
        glm::tvec3<float>                      mSortOrigin;                  // off+272
        bool                                   mIsUI;                        // off+284
        bool                                   mIsDrawingFirstPersonObjects; // off+285
        bool                                   mIsDrawingInLevelCubeMap;     // off+286
        bool                                   mEnvironmentalText;           // off+287

#if MC_VERSION == v1_21_60
        bool mUnk288; // off+288
#endif

        mce::Color mCurrentColorUniform; // off+288
        mce::Color mDarkenUniform;       // off+304

        mce::MaterialPtr mOldMat; // off+320

        const mce::GlobalConstantBuffers &mConstants;  // off+336
        glm::tvec4<float>                 mUVAnim;     // off+344
        float                             mHudOpacity; // off+360
        mce::framebuilder::FogDescription mFog;        // off+364

        std::optional<glm::tvec4<float>> mClipRegion;     // off+392
        const uint16_t                   mViewId;         // off+412
        const dragon::RenderMetadata     mRenderMetadata; // off+416
    };

} // namespace mce::framebuilder