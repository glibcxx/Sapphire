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
    }

} // namespace mce::framebuilder::bgfxbridge