#pragma once

#include "SDK/api/src-deps/Core/CheckedResourceService/CheckedResourceService.h"
#include "SDK/api/src-external/RenderDragon/Materials/CompiledMaterialManager.h"
#include "SDK/api/src-external/RenderDragon/Materials/Material.h"

namespace dragon {

    // size: 72
    struct MaterialResourceServiceContext {
        gsl::not_null<dragon::materials::CompiledMaterialManager *> mCompiledMaterialManager; // off+0
        std::function<uintptr_t(uintptr_t)>                         mUnk8;                    // off+8
    };

    // size: 24 (1.21.50)
    class ResolvedMaterialResource {
    public:
        std::unique_ptr<materials::Material>                   mRuntimeMaterial;  // off+0
        std::shared_ptr<materials::CompiledMaterialDefinition> mCompiledMaterial; // off+8
    };

    using MaterialResourceService = mce::CheckedResourceService<ResolvedMaterialResource>;

} // namespace dragon

// size: 128
template <>
class mce::CheckedResourceService<dragon::ResolvedMaterialResource> {
public:
    using ResourceServiceContextMember = dragon::MaterialResourceServiceContext;

    using FactoryTrackerPtr = std::weak_ptr<
        mce::ResourceBlockTemplate<dragon::ResolvedMaterialResource, mce::UncheckedHandleTracker, dragon::BufferDescription>>;

    using FactoryTracker = mce::SimpleResourceTracker<
        std::shared_ptr<mce::ResourceBlockTemplate<dragon::ResolvedMaterialResource, mce::UncheckedHandleTracker, dragon::BufferDescription>>,
        FactoryTrackerPtr>;

    using ResourcePtr = mce::ResourcePointer<dragon::ResolvedMaterialResource>;

    using ResourceServiceContext = ResourceServiceContextMember;

    FactoryTracker            mResourceTracker;        // off+0
    ResourceServiceContext    mResourceServiceContext; // off+48
    mce::ResourceServiceState mState;                  // off+120
};