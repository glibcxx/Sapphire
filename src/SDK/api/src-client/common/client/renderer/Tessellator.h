#pragma once

#include "SDK/api/src/common/CommonTypes.h"
#include "SDK/api/src/common/world/phys/Vec3.h"
#include "SDK/api/src-deps/Core/Math/Color.h"
#include "SDK/api/src-deps/Core/Utility/PrimitiveTypes.h"
#include "SDK/api/src-deps/MinecraftRenderer/Renderer/Mesh.h"

namespace mce {
    class MaterialPtr;
    class TexturePtr;
} // namespace mce

// size: 16
struct TessellatorQuadInfo {
    FacingID facing;   // off+0
    FacingID twoFace;  // off+1
    Vec3     centroid; // off+4
};

// size: 592 (1.21.2), 632 (1.21.50,1.21.60)
class Tessellator {
public:
    ;                             // off+(1.21.50)
    bool          mIsFormatFixed; // off+0
    mce::MeshData mMeshData;      // off+8
#if MC_VERSION >= v1_21_50
    bool mHasNormals; // off+296
#endif
    std::optional<glm::tvec4<float>> mNextNormal;        // off+300
    std::optional<glm::tvec2<float>> mNextUV[3];         // off+320
    std::optional<uint32_t>          mNextColor;         // off+356
    std::optional<uint16_t>          mNextBoneId;        // off+364
    std::optional<uint16_t>          mNextPBRTextureIdx; // off+368
#if MC_VERSION >= v1_21_50
    std::optional<uint32_t> mNextMERS; // off+372
#endif
    bool                             mIndexPhase;               // off+380
    Vec3                             mPostTransformOffset;      // off+384
    Vec3                             mPostTransformScale;       // off+396
    FacingID                         mQuadFacing;               // off+408
    bool                             mQuadTwoSided;             // off+409
    std::vector<TessellatorQuadInfo> mQuadInfoList;             // off+416
    Vec3                             mFaceCenterAccumulator;    // off+440
    int                              mCurQuadVertex;            // off+452
    bool                             mApplyTransform;           // off+456
    glm::tmat4x4<float>              mTransformMatrix;          // off+460
    bool                             mNoColor;                  // off+524
    bool                             mVoidBeginEnd;             // off+525
    bool                             mForceTessellateIntercept; // off+526
    std::function<void(const Tessellator &, const mce::MaterialPtr &, const mce::TexturePtr &)>
                                              mInterceptTessellator;  // off+528
    uint32_t                                  mCount;                 // off+592
    __int32                                   mUnk596;                // off+596
    bool                                      mTessellating;          // off+600
    bool                                      mBuildFaceData;         // off+601
    std::unique_ptr<mce::Mesh>                mPreGeneratedMesh;      // off+608
    std::weak_ptr<mce::BufferResourceService> mBufferResourceService; // off+616

    enum class UploadMode : int32_t {
        Buffered = 0,
        Manual = 1,
        Never = 2,
    };

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x83\xC3\x00\x00\x8D\x00\xE0\x04")
    SDK_API void begin(const mce::PrimitiveMode mode, const int numVertices, const bool buildFaceData = false);

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\xF3\x0F\x10\x5B\x00\xF3\x0F\x10\x13")
    SDK_API void vertex(float x, float y, float z);

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "\x48\x83\xEC\x00\x80\xB9\x00\x00\x00\x00\x00\x0F\x57\xE4")
    SDK_API void vertexUV(float x, float y, float z, float u, float v);

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "\x80\xB9\x00\x00\x00\x00\x00\x4C\x8B\xC1\x75")
    SDK_API void color(float r, float g, float b, float a);

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x90\x49\x8B\x45\x00\x48\xC7\x44\x24")
    SDK_API mce::Mesh end(UploadMode uploadMode, std::string_view debugName, bool a3);

    void vertex(const Vec3 &pos) {
        this->vertex(pos.x, pos.y, pos.z);
    }

    void vertexUV(const Vec3 &pos, const Vec2 &uv) {
        this->vertexUV(pos.x, pos.y, pos.z, uv.x, uv.y);
    }

    void vertexUV(const Vec3 &pos, float u, float v) {
        this->vertexUV(pos.x, pos.y, pos.z, u, v);
    }

    void quadFacing(FacingID facing) {
        if (!this->mCurQuadVertex)
            this->mQuadFacing = facing;
    }

    void quadTwoSided(bool twoSided) {
        if (!this->mCurQuadVertex)
            this->mQuadTwoSided = twoSided;
    }

    void color(int r, int g, int b, int a) {
        return this->color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }

    void color(const mce::Color &color) {
        return this->color(color.r, color.g, color.b, color.a);
    }

    void color(int color) {
        return this->color(color >> 24, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
    }
};