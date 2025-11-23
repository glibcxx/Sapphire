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
            using Bind = sapphire::bind::Fn<
                sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x4C\x8B\xD8\x48\x8B\x8D"_sig,
                &makeMeshFilter>;
            return Bind::origin(allocator, buffer, clientBuffer, vertexCount, indicesOffset, indicesCount);
        }

    } // namespace meshutils

    std::optional<DFC::MeshFilter> makeMeshFilter(
        const mce::framebuilder::bgfxbridge::EntityCreationContext &entityContext,
        const DFC::MaterialFilter                                  &materialFilter,
        const mce::Mesh                                            &mesh
    ) {
        using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x41\xB2"_sig,
#elif MC_VERSION == v1_21_50 
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\xB3\x00\x66\xC7\x44\x24"_sig,
#elif MC_VERSION == v1_21_60
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\x4B"_sig,
#endif
            &makeMeshFilter>;
        return Bind::origin(entityContext, materialFilter, mesh);
    }

} // namespace mce::framebuilder::bgfxbridge