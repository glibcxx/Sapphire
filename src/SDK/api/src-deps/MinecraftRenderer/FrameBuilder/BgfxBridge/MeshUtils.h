#pragma once

#include <optional>
#include "SDK/api/src-external/RenderDragon/FrameObject/Components/MeshFilter.h"
#include "SDK/api/src-external/RenderDragon/FrameObject/Components/MaterialFilter.h"
#include "SDK/api/src-deps/MinecraftRenderer/FrameBuilder/BgfxBridge/BgfxFrameExtractor.h"
#include "SDK/api/src-deps/MinecraftRenderer/Renderer/Mesh.h"

namespace mce::framebuilder::bgfxbridge {

    namespace meshutils {

        SDK_API DFC::MeshFilter makeMeshFilter(
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
        );
        MARK_HOOKABLE(&makeMeshFilter)

    } // namespace meshutils

    SDK_API std::optional<DFC::MeshFilter> makeMeshFilter(
        const mce::framebuilder::bgfxbridge::EntityCreationContext &entityContext,
        const DFC::MaterialFilter                                  &materialFilter,
        const mce::Mesh                                            &mesh
    );
    MARK_HOOKABLE(&makeMeshFilter)

} // namespace mce::framebuilder::bgfxbridge
