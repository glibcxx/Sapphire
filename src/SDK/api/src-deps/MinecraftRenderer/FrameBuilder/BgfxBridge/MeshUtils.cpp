#include "MeshUtils.h"

namespace mce::framebuilder::bgfxbridge {

    namespace meshutils {
        DFC::MeshFilter makeMeshFilter(
            void                                        *allocator,
            dragon::rendering::BgfxPrimitiveIndexBuffer &buffer,
            const mce::ServerResourcePointer<
                std::variant<
                    std::monostate,
                    mce::Buffer,
                    mce::ClientResourcePointer<dragon::ResolvedVertexBufferResource>,
                    mce::ClientResourcePointer<dragon::ResolvedIndexBufferResource>>> &clientBuffer,
            const int                                                                  vertexCount,
            uint32_t                                                                   indicesOffset,
            uint32_t                                                                   indicesCount
        ) {
            using Hook = core::ApiLoader<
                "\xE8\x00\x00\x00\x00\x4C\x8B\xD8\x48\x8B\x8D"_sig,
                &makeMeshFilter,
                core::deRefCall>;
            return Hook::origin(allocator, buffer, clientBuffer, vertexCount, indicesOffset, indicesCount);
        }

    } // namespace meshutils

    std::optional<DFC::MeshFilter> makeMeshFilter(
        const mce::framebuilder::bgfxbridge::EntityCreationContext &entityContext,
        const DFC::MaterialFilter                                  &materialFilter,
        const mce::Mesh                                            &mesh
    ) {
        using Hook = core::ApiLoader<
            "\xE8\x00\x00\x00\x00\x90\xB3\x00\x66\xC7\x44\x24"_sig,
            &makeMeshFilter,
            core::deRefCall>;
        return Hook::origin(entityContext, materialFilter, mesh);
    }

} // namespace mce::framebuilder::bgfxbridge