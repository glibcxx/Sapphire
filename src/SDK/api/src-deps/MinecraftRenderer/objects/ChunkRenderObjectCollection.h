#pragma once

struct RenderChunkDirectIndexData;
struct RenderChunkDirectVertexData;
struct ChunkLayerRenderObject;
struct ChunkRenderData;
namespace mce {
    class IndexBufferContainer;
    class Mesh;
    class ServerTexture;
    struct PointLight;
} // namespace mce

// size: 88
struct ChunkRenderData {
    glm::tvec3<int> mPosition;      // off+0
    double          mReadyTimeDiff; // off+16
    const std::variant<
        std::monostate,
        std::shared_ptr<mce::IndexBufferContainer>,
        std::shared_ptr<RenderChunkDirectIndexData>>
        mChunkIndices; // off+24
    const std::variant<
        std::monostate,
        std::shared_ptr<mce::Mesh>,
        std::shared_ptr<RenderChunkDirectVertexData>>
                                     mChunkMesh;   // off+48
    gsl::span<const mce::PointLight> mPointLights; // off+72
};

// size: 1352
struct ChunkRenderObjectCollection {
    std::vector<mce::ServerTexture>     mTextures;                      // off+0
    std::vector<ChunkRenderData>        mChunkQueue;                    // off+24
    std::vector<ChunkLayerRenderObject> mTerrainLayerChunkQueue[3][18]; // off+32
    uint32_t                            mMaximumChunkVertexCount;       // off+1344

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x0F\x2F\x3F")
    SDK_API size_t addToChunkQueue(
        const std::variant<
            std::monostate,
            std::shared_ptr<mce::IndexBufferContainer>,
            std::shared_ptr<RenderChunkDirectIndexData>> &chunkIndices,
        const std::variant<
            std::monostate,
            std::shared_ptr<mce::Mesh>,
            std::shared_ptr<RenderChunkDirectVertexData>> &chunkMesh,
        const glm::tvec3<int>                             &chunkPos,
        double                                             timeDiff,
        gsl::span<const mce::PointLight>                   pointLights
    );
};