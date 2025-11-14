#pragma once

#include "SDK/api/src/common/world/Flip.h"
#include "SDK/api/src/common/world/phys/AABB.h"
#include "SDK/api/src/common/world/level/block/BlockRenderLayer.h"
#include "SDK/api/src-client/common/client/renderer/texture/TextureUVCoordinateSet.h"
#include "SDK/api/src-client/common/client/renderer/MaterialPtr.h"
#include "SDK/api/src-deps/Core/Math/Color.h"
#include "SDK/api/src-deps/MinecraftRenderer/framebuilder/FrameLightingModelCapabilities.h"
#include "BlockTessellatorCustomExtraData.h"
#include "BlockTessellatorCache.h"

class Block;
class BlockSource;
namespace mce {
    class Mesh;
}
enum class BakedBlockLightType : int;

// size: 32
struct BlockTessellatorBlockInWorld {
    const Block     &block; // off+0
    BlockRenderLayer layer; // off+8
    BlockPos         pos;   // off+12
    const void      *owner; // off+24
};

// size: 294488
class BlockTessellator {
public:
    // size: 192
    struct AocBlockFaceData {
        std::array<mce::Color, 4> aocCL{};  // off+0
        std::array<mce::Color, 4> aocCH{};  // off+64
        std::array<Vec2, 4>       aocUVL{}; // off+128
        std::array<Vec2, 4>       aocUVH{}; // off+160
    };

    // size: 20
    struct UVOverride {
        uint16_t pbrTextureDataHandle = -1; // off+0
        float    u0 = 0.0f;                 // off+4
        float    u1 = 0.0f;                 // off+8
        float    v0 = 0.0f;                 // off+12
        float    v1 = 0.0f;                 // off+16
    };

    using AocBlockData = std::array<AocBlockFaceData, 6>;

    bool                                                        mRenderingExtra;                   // off+0
    bool                                                        mUseNormals;                       // off+1
    BlockSource                                                *mRegion;                           // off+8
    mce::framebuilder::FrameLightingModelCapabilities           mLightingModelCapabilities;        // off+16
    TextureUVCoordinateSet                                      mFixedTexture;                     // off+24
    bool                                                        mUseFixedTexture;                  // off+120
    bool                                                        mUseOccluder;                      // off+121
    bool                                                        _tmpUseRegion;                     // off+122
    bool                                                        mXFlipTexture;                     // off+123
    int                                                         mRenderingLayer;                   // off+124
    bool                                                        mRenderingGUI;                     // off+128
    bool                                                        mShouldEmitPointLights;            // off+129
    bool                                                        mUseFixedColor;                    // off+130
    std::array<mce::Color, 6>                                   mFixedColors;                      // off+132
    bool                                                        mApplyAmbientOcclusion;            // off+228
    BakedBlockLightType                                         mBakedLighting;                    // off+232
    bool                                                        mForExport;                        // off+236
    bool                                                        mUnk237;                           // off+237
    mce::Color                                                  mAoColors[8];                      // off+240
    BrightnessPair                                              mTc[4];                            // off+368
    std::array<Flip, 6>                                         mFlipFace;                         // off+376
    std::array<UVOverride, 6>                                   mTextureOverride;                  // off+400
    const Block                                                *mAmbientOcclusionCacheBlocks[125]; // off+520
    mce::MaterialPtr                                            mItemMat;                          // off+1520
    AABB                                                        mCurrentShapeBB;                   // off+1536
    bool                                                        mCurrentShapeSet;                  // off+1560
    const Block                                                *mCurrentShapeBlock;                // off+1568
    BlockPos                                                    mCurrentShapeBlockPos;             // off+1576
    float                                                       mCurrentScale;                     // off+1588
    std::unordered_map<unsigned int, mce::Mesh>                 mBlockMeshes;                      // off+1592
    std::unordered_map<BlockTessellatorBlockInWorld, mce::Mesh> mInWorldBlockMeshes;               // off+1656
    BlockTessellatorCache                                       mBlockCache;                       // off+1720
    std::function<const Block &(const BlockPos &)>              mCachedGetBlock;                   // off+273752
    alignas(8) char mUnk273816[19288];                                                             // off+273816
    std::optional<mce::Color>            mUnk293104;                                               // off+293104
    bool                                 mForceOpaque;                                             // off+293124
    std::vector<int /*mce::PointLight*/> mPointLights;                                             // off+293128
    float                                mUnk293152[6];                                            // off+293152
    AocBlockData                         mAocBlockData;                                            // off+293176
    BlockTessellatorCustomExtraData::Map mBlockExtraDataMap;                                       // off+294328
    __int64                              _fill294456[4];                                           // off+294456

    SDK_API BlockTessellator(BlockSource *level);
#pragma SPHR_LINKER_SYM_ALIAS("??0BlockTessellator@@QEAA@PEAVBlockSource@@@Z", "?ctor@BlockTessellator@@QEAAPEAV1@PEAVBlockSource@@@Z")

    SDK_API BlockTessellator *ctor(BlockSource *level);

    SDK_API ~BlockTessellator() noexcept;
#pragma SPHR_LINKER_SYM_ALIAS("??1BlockTessellator@@QEAA@XZ", "?dtor@BlockTessellator@@QEAAXXZ")

    SDK_API void dtor() noexcept;
};