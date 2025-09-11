#pragma once

#include "SDK/api/src-deps/MinecraftRenderer/Renderer/MeshData.h"

class TerrainLayer;
struct TessellatorQuadInfo;

namespace ClientBlockPipeline {

    // size: 264
    struct SimpleMesh {
        mce::MeshData                    mMeshData;     // off+0
        std::vector<TessellatorQuadInfo> mQuadInfoList; // off+240
    };

    // size: 64
    struct NamedMeshStreams {
        std::unordered_map<const TerrainLayer *, SimpleMesh> mStreams; // off+0
    };

    // size: 64
    class BlockTessellatorPipeline {
    public:
        NamedMeshStreams mMeshStreams; // off+0
    };

} // namespace ClientBlockPipeline