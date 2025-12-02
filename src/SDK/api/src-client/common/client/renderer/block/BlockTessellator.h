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
class Tessellator;
class LightTexture;
class BlockOccluder;
struct AirAndSimpleBlockBits;
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

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x4C\x8B\xF2\x48\x8B\xF1")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\xC0\x00\x00\x00\x4C\x8B\xFA\x48\x8B\xF9\x48\x89\x4D")
    SDK_API BlockTessellator *ctor(BlockSource *level);

    SDK_API ~BlockTessellator() noexcept;
#pragma SPHR_LINKER_SYM_ALIAS("??1BlockTessellator@@QEAA@XZ", "?dtor@BlockTessellator@@QEAAXXZ")

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xD9\x48\x81\xC1\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8D\x8B\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8D\x8B\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8D\x8B\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8D\x8B\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8D\x8B\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8D\xBB")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xD9\x48\x81\xC1\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\x8B\x00\x00\x00\x00\x33\xED\x48\x85\xC9\x74\x00\x48\x8B\x93")
    SDK_API void dtor() noexcept;

    SPHR_DECL_API("1.21.2", "\x48\x8B\xC4\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4D\x8B\xF1\x49\x8B\xC0")
    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x49\x8D\x4E\x00\x4C\x89\x7D")
    SPHR_DECL_API("1.21.60", "call", "\xE8\x00\x00\x00\x00\x49\x8D\x4C\x24\x00\x4C\x89\x75")
    SDK_API mce::Mesh &getMeshForBlockInWorld(
        Tessellator     &tessellator,
        const Block     &block,
        BlockPos        &blockPos,
        BlockRenderLayer layer,
        const void      *owner,
        bool             useOccluder,
        bool             tmpUseRegion,
        LightTexture    &lightTexture
    );

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xFA\x48\x8B\xD9\x48\x3B\xCA\x74\x00\xE8\x00\x00\x00\x00\x8B\x07\x89\x03\x48\x8B\x47\x00\x48\x8B\x4B\x00\x48\x89\x43\x00\x48\x8B\x47\x00\x48\x89\x4F\x00\x48\x8B\x4B\x00\x48\x89\x43\x00\x48\x8B\x47\x00\x48\x89\x4F\x00\x48\x8B\x4B\x00\x48\x89\x43\x00\x48\x8B\x47\x00\x48\x89\x4F\x00\x48\x8B\x4B\x00\x48\x89\x43\x00\x48\x8B\x47\x00\x48\x89\x4F\x00\x48\x8B\x4B\x00\x48\x89\x43\x00\x48\x8B\x47\x00\x48\x89\x4F\x00\x48\x8B\x4B\x00\x48\x89\x43\x00\x48\x8B\x47\x00\x48\x89\x4F\x00\x48\x8B\x4B\x00\x48\x89\x43\x00\x48\x89\x4F\x00\x48\x8B\xC3\x48\x8B\x5C\x24\x00\x48\x83\xC4\x00\x5F\xC3\xCC\xCC\xCC\xCC\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xF9\xE8")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x48\x8B\xF2\x48\x8B\xD9\x33\xED\x48\x3B\xCA\x0F\x84")
    SDK_API void setBlockExtraDataMap(BlockTessellatorCustomExtraData::Map map);

    SPHR_DECL_API("1.21.2", "\x40\x53\x55\x56\x57\x41\x56\x48\x83\xEC\x00\x80\x79")
    SPHR_DECL_API("1.21.50", "\x48\x8B\xC4\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x68\x00\x48\x81\xEC\x00\x00\x00\x00\x80\xB9")
    SPHR_DECL_API("1.21.60", "call", "\xE8\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x33\xDB\xE9")
    SDK_API bool tessellateInWorld(
        Tessellator &tessellator, const Block &block, const BlockPos &pos, bool renderBothSides
    );

    SPHR_DECL_API("1.21.2", "\x40\x55\x56\x57\x41\x54\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x80\xB9\x00\x00\x00\x00\x00\x49\x8B\xE9")
    SPHR_DECL_API("1.21.50", "\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x80\xB9")
    SPHR_DECL_API("1.21.60", "\x40\x55\x56\x57\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05")
    SDK_API bool tessellateBlockInWorld(
        Tessellator                 &tessellator,
        const Block                 &block,
        const BlockPos              &pos,
        int                          faces,
        const AirAndSimpleBlockBits *airAndSimpleBlocks
    );

    SPHR_DECL_API("1.21.2", "\x48\x8B\xC4\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x44\x0F\x29\x88")
    SPHR_DECL_API("1.21.50", "\x48\x8B\xC4\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x44\x0F\x29\x40\x00\x44\x0F\x29\x88\x00\x00\x00\x00\x44\x0F\x29\x90\x00\x00\x00\x00\x44\x0F\x29\xA8")
    SPHR_DECL_API("1.21.60", "\x48\x8B\xC4\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x44\x0F\x29\x40\x00\x44\x0F\x29\x88\x00\x00\x00\x00\x44\x0F\x29\x90\x00\x00\x00\x00\x44\x0F\x29\xB0")
    SDK_API bool tessellateBlockInWorldFlat(
        Tessellator      &tessellator,
        const Block      &block,
        const BlockPos   &pos,
        const mce::Color &base,
        BlockOccluder    *occluder,
        int               faces,
        int               forcedVariant
    );

    SPHR_DECL_API("1.21.2", "\x40\x55\x56\x57\x41\x54\x41\x55\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85")
    SPHR_DECL_API("1.21.50", "\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\xB4\x24")
    SPHR_DECL_API("1.21.60", "\x40\x55\x53\x56\x57\x41\x54\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\xB4\x24")
    SDK_API bool tessellateBlockInWorldWithAmbienceOcclusion(
        Tessellator      &tessellator,
        const Block      &block,
        BlockPos         *p,
        const mce::Color &base,
        BlockOccluder    *occluder,
        int               faces,
        int               forcedVariant
    );

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x41\x56\x48\x83\xEC\x00\x48\x8B\x7C\x24\x00\x49\x8B\xD8")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x48\x83\xEC\x00\x48\x8B\x7C\x24")
    SDK_API const TextureUVCoordinateSet &_getTexture(
        const BlockPos &pos, const Block &block, FacingID face, int forcedVariant, BlockGraphics *blockGraphics
    ) const;

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x80\xB9\x00\x00\x00\x00\x00\x48\x8B\xD9\x0F\x85")
    SDK_API const AABB &getCurrentShape();
};