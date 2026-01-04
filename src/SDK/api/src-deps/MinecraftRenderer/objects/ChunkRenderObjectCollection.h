#pragma once

struct RenderChunkDirectIndexData;
struct RenderChunkDirectVertexData;
namespace mce {
    class IndexBufferContainer;
    class Mesh;
    struct PointLight;
} // namespace mce

struct ChunkRenderObjectCollection {
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