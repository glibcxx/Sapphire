#pragma once

#include "BlockShape.h"
#include "SDK/api/src-client/common/client/renderer/texture/IsotropicFaceData.h"
#include "SDK/api/src-client/common/client/renderer/texture/TextureUVCoordinateSet.h"
#include "SDK/api/src/common/world/level/block/BlockRenderLayer.h"
#include "SDK/api/src/common/world/phys/AABB.h"

class Block;
class BlockSource;
class Random;
struct TextureItem;
namespace BlockGeometry {
    class TessellatedModel;
    struct Model;
} // namespace BlockGeometry

// size: 184 (1.21.50)
class BlockGraphics {
public:
    // size: 64
    struct ModelItem {
        std::string                            name;           // off+0
        const BlockGeometry::TessellatedModel *model;          // off+32
        std::vector<uint64_t>                  textureIndices; // off+40
    };

    IsotropicFaceData                                      mIsotropicFaceData;     // off+8
    const Block                                           *mBlock;                 // off+16
    BlockRenderLayer                                       mRenderLayer;           // off+24
    BlockShape                                             mBlockShape;            // off+28
    bool                                                   mAnimatedTexture;       // off+32
    float                                                  mBrightnessGamma;       // off+36
    bool                                                   mFancy;                 // off+40
    bool                                                   mAllowSame;             // off+41
    std::string                                            mUnk48;                 // off+48
    AABB                                                   mVisualShape;           // off+80
    std::vector<TextureItem>                               mTextureItems;          // off+104
    uint64_t                                               mIconTextureIndex;      // off+128
    std::vector<std::vector<const BlockGeometry::Model *>> mBlockModelVariants;    // off+136
    std::vector<std::vector<ModelItem>>                    mTessellatedModelParts; // off+160

    // vtb+0
    virtual ~BlockGraphics();

    // vtb+1
    virtual BlockRenderLayer getRenderLayer(BlockSource &region, const BlockPos &pos) const;

    // vtb+2
    virtual int getColor(int auxData) const;

    // vtb+3
    virtual int getColor(BlockSource &region, const BlockPos &pos) const;

    // vtb+4
    virtual bool isSeasonTinted(BlockSource &region, const BlockPos &p) const;

    // vtb+5
    virtual void onGraphicsModeChanged(bool fancy, bool fancyBubbles);

    // vtb+6
    virtual int getExtraRenderLayers();

    // vtb+7
    virtual const TextureUVCoordinateSet &getCarriedTexture(uint64_t textureSlot, int blockVariant) const;

    // vtb+8
    virtual void animateTick(BlockSource &region, const BlockPos &pos, Random &random);

    // vtb+9
    virtual Vec3 randomlyModifyPosition(const BlockPos &pos) const;

    // vtb+10
    virtual Vec3 randomlyModifyPosition(const BlockPos &pos, int &seed) const;

    // vtb+11
    virtual void setVisualShape(const Vec3 &min, const Vec3 &max);

    // vtb+12
    virtual void setVisualShape(const AABB &shape);

    SPHR_DECL_API("1.21.2", "\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x80\xB9\x00\x00\x00\x00\x00\x74\x00\x4C\x8D\x81")
    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x4C\x8B\xF0\x48\x85\xC0\x0F\x84\x00\x00\x00\x00\x48\x8B\x48\x00\x48\x2B\x48\x00\x48\xB8")
    SPHR_DECL_API("1.21.60", "\x48\x8B\x81\x00\x00\x00\x00\x4C\x8B\xC1\x48\x85\xC0\x0F\x85")
    static const BlockGraphics *getForBlock(const Block &block);
};