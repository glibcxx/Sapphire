#pragma once

#include "SDK/api/src-external/RenderDragon/Endstone/Guarded.hpp"

namespace mce {
    class Mesh;
} // namespace mce
namespace dragon::rendering {
    class BgfxPrimitiveIndexBuffer;
}

namespace mce::framebuilder::bgfxbridge {

    struct RayTraceableMesh;
    struct EntityCreationContext;
    struct RayTraceableMeshKey;
    struct RayTraceableMeshKeyHasher;

    namespace rtxutils {

        SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x4C\x89\x6C\x24\x00\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05")
        SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x48\x89\x54\x24\x00\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05")
        SDK_API const RayTraceableMesh *tryGetRayTraceableMesh(
            const EntityCreationContext &entityContext,
            const mce::Mesh             &staticMesh,
            dragon::guarded::Guarded<
                std::unordered_map<
                    RayTraceableMeshKey,
                    std::unique_ptr<RayTraceableMesh>,
                    RayTraceableMeshKeyHasher>>         &meshCache,
            dragon::rendering::BgfxPrimitiveIndexBuffer &quadIndexBuffer
        );
    } // namespace rtxutils

} // namespace mce::framebuilder::bgfxbridge