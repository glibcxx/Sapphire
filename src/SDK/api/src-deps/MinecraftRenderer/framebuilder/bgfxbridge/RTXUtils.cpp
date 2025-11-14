#include "RTXUtils.h"

const mce::framebuilder::bgfxbridge::RayTraceableMesh *mce::framebuilder::bgfxbridge::rtxutils::tryGetRayTraceableMesh(
    const mce::framebuilder::bgfxbridge::EntityCreationContext &entityContext,
    const mce::Mesh                                            &staticMesh,
    dragon::guarded::Guarded<
        std::unordered_map<
            mce::framebuilder::bgfxbridge::RayTraceableMeshKey,
            std::unique_ptr<mce::framebuilder::bgfxbridge::RayTraceableMesh>,
            mce::framebuilder::bgfxbridge::RayTraceableMeshKeyHasher>> &meshCache,
    dragon::rendering::BgfxPrimitiveIndexBuffer                        &quadIndexBuffer
) {
    using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
        "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x4C\x89\x6C\x24\x00\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x48\x89\x54\x24\x00\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05"_sig,
#endif
        &mce::framebuilder::bgfxbridge::rtxutils::tryGetRayTraceableMesh>;
    return Hook::origin(entityContext, staticMesh, meshCache, quadIndexBuffer);
}