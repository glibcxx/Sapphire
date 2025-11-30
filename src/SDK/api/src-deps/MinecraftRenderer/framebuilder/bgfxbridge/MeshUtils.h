#pragma once

#include <optional>
#include "SDK/api/src-external/RenderDragon/FrameObject/Components/MeshFilter.h"
#include "SDK/api/src-external/RenderDragon/FrameObject/Components/MaterialFilter.h"
#include "SDK/api/src-deps/MinecraftRenderer/FrameBuilder/BgfxBridge/BgfxFrameExtractor.h"
#include "SDK/api/src-deps/MinecraftRenderer/Renderer/Mesh.h"

namespace mce::framebuilder::bgfxbridge {

    namespace meshutils {

        SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "\xE8\x00\x00\x00\x00\x4C\x8B\xD8\x48\x8B\x8D")
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

    } // namespace meshutils

    SAPPHIRE_API("1.21.2", "\xE8\x00\x00\x00\x00\x90\x41\xB2")
    SAPPHIRE_API("1.21.50", "\xE8\x00\x00\x00\x00\x90\xB3\x00\x66\xC7\x44\x24")
    SAPPHIRE_API("1.21.60", "\xE8\x00\x00\x00\x00\x90\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x8B\x4B")
    SDK_API std::optional<DFC::MeshFilter> makeMeshFilter(
        const mce::framebuilder::bgfxbridge::EntityCreationContext &entityContext,
        const DFC::MaterialFilter                                  &materialFilter,
        const mce::Mesh                                            &mesh
    );

} // namespace mce::framebuilder::bgfxbridge
