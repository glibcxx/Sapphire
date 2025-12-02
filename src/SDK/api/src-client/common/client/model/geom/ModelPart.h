#pragma once

#include "SDK/api/src-deps/Core/Utility/StringHash.h"
#include "SDK/api/src/common/world/phys/AABB.h"
#include "SDK/api/src-client/common/client/model/Geometry.h"
#include "SDK/api/src-deps/Core/Resource/ResourceHelper.h"
#include "TextureOffset.h"

class ModelPartLocator;
class Cube;
class RenderController;
class ExpressionNode;

#if MC_VERSION <= v1_21_50
enum class BoneAnimationRelativeMode : int {
    Parent = 0,
    Entity = 1,
};
#else
enum class BoneAnimationRelativeMode : uint8_t {
    Parent = 0,
    Entity = 1,
};
#endif

// size: 880 (1.21.2), 808 (1.21.50), 800 (1.21.60)
class ModelPart {
public:
    // size: 136
    struct TextureMesh {
        std::string      mFriendlyName;     // off+0
        ResourceLocation mResourceLocation; // off+32
        Vec3             mPosition;         // off+88
        Vec3             mLocalPivot;       // off+100
        Vec3             mRotation;         // off+112
        Vec3             mScale;            // off+124
    };

#if MC_VERSION == v1_21_2
    // size: 296
    class Unk176 {
    public:
        class Unk264 {
        public:
            virtual ~Unk264() = 0;
        };
        uint32_t                                     mUnk0;     // off+0
        float                                        mUn4;      // off+4
        uint64_t                                     _fill8[3]; // off+8
        std::variant<std::aligned_storage_t<128, 8>> mUnk32;    // off+32
        std::vector<Unk176>                          mUnk168;   // off+168
        uint64_t                                     mUnk192;   // off+192
        std::string                                  mUnk200;   // off+200
        std::string                                  mUnk232;   // off+232
        std::vector<std::unique_ptr<Unk264>>         mUnk264;   // off+264
        uint64_t                                     mUnk288;   // off+288
    };
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    // size: 224
    class Unk176 {
    public:
        class Unk136 {
        public:
            virtual ~Unk136() = 0;
        };
        uint32_t                                    mUnk0;     // off+0
        float                                       mUn4;      // off+4
        uint64_t                                    _fill8[3]; // off+8
        std::variant<std::aligned_storage_t<72, 8>> mUnk32;    // off+32
        std::vector<Unk176>                         mUnk112;   // off+112
        std::vector<std::unique_ptr<Unk136>>        mUnk136;   // off+136
        std::string                                 mUnk160;   // off+160
        std::string                                 mUnk192;   // off+192
    };
#endif

    ;                                                          // off+(1.21.50)
    HashedString                  mName;                       // off+0
    Vec3                          mPos;                        // off+48
    Vec3                          mOffset;                     // off+60
    Vec3                          mRot;                        // off+72
    Vec3                          mScale;                      // off+84
    Vec3                          mWorldPos;                   // off+96
    AABB                          mAABB;                       // off+108
    int                           mGroupIndex;                 // off+132
    int                           mPartIndex;                  // off+136
    int                           mMeshIndex;                  // off+140
    int                           mBoneOrientationIndex;       // off+144
    std::vector<ModelPartLocator> mLocators;                   // off+152
    Unk176                        mUnk176;                     // off+176
    BoneAnimationRelativeMode     mRotationRelativeMode;       // off+400
    bool                          mDebug;                      // off+404
    const int                     IndexNotSet = 0x80000000;    // off+408
    bool                          mMirror = false;             // off+412
    bool                          mVisible = true;             // off+413
    int                           mSkinnedMeshGroupIdentifier; // off+416
    std::vector<Cube>             mCubes;                      // off+424
    std::vector<TextureMesh>      mTextureMeshes;              // off+448
    Geometry::NodePolyMesh        mPolyMesh;                   // off+472
    ModelPart                    *mParent;                     // off+600
    std::vector<ModelPart *>      mChildren;                   // off+608
    Vec2                          mTexSize;                    // off+632
    TextureOffset                 mTexOffs;                    // off+640
    bool                          mNeverRender;                // off+656
    Vec3                          mOriginPos;                  // off+660
    bool                          mHasColor;                   // off+672

    std::unordered_map<const RenderController *, ExpressionNode> mIsVisibleMap;          // off+680
    std::unordered_map<const RenderController *, ExpressionNode> mMaterialExpressionMap; // off+744

    SDK_API ModelPart(int xTexOffs, int yTexOffs, int texWidth, int texHeight);
#pragma SPHR_LINKER_SYM_ALIAS("??0ModelPart@@QEAA@HHHH@Z", "?ctor@ModelPart@@QEAAPEAV1@HHHH@Z")

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x90\x4C\x8D\x00\x00\x00\x00\x00\xC7\x44\x24\x20\x20\x00\x00\x00\x41\xB9\x40\x00\x00\x00\x45\x33\xC0\x33\xD2\x49\x8B")
    SDK_API ModelPart *ctor(int xTexOffs, int yTexOffs, int texWidth, int texHeight);

    SDK_API ~ModelPart();
#pragma SPHR_LINKER_SYM_ALIAS("??1ModelPart@@QEAA@XZ", "?dtor@ModelPart@@QEAAXXZ")

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\xF3\x0F\x11\x7C\x24\x00\xC7\x44\x24")
    SDK_API void dtor() noexcept;
};