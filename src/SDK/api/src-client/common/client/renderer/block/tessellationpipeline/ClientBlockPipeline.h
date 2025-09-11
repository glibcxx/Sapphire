#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src-client/common/client/renderer/texture/TextureUVCoordinateSet.h"

class TerrainLayer;

namespace ClientBlockPipeline {

    // size: 136
    class Material : public Bedrock::EnableNonOwnerReferences {
    public:
        const TerrainLayer    *mTerrainLayer;     // off+24
        TextureUVCoordinateSet mUvSet;            // off+32
        bool                   mAmbientOcclusion; // off+128
        bool                   mFaceDimming;      // off+129
    };

    class Description {};

    // size: 24
    class MaterialRepository {
    public:
        std::vector<std::shared_ptr<const Material>> mMaterials; // off+0
    };

} // namespace ClientBlockPipeline