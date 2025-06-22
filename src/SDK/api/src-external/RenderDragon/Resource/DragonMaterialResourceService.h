#pragma once

#include "SDK/api/src-deps/Core/CheckedResourceService/CheckedResourceService.h"
#include "SDK/api/src-external/RenderDragon/Materials/CompiledMaterialManager.h"

namespace dragon {

    // size: 72
    struct MaterialResourceServiceContext {
        gsl::not_null<dragon::materials::CompiledMaterialManager *> mCompiledMaterialManager; // off+0
        std::function<uintptr_t(uintptr_t)>                         mUnk8;                    // off+8
    };

    class ResolvedMaterialResource {};

    using MaterialResourceService = mce::CheckedResourceService<ResolvedMaterialResource>;

} // namespace dragon

// size: 128
template <>
class mce::CheckedResourceService<dragon::ResolvedMaterialResource> {
public:
    using ResourceServiceContextMember = dragon::MaterialResourceServiceContext;

    using FactoryTracker = mce::SimpleResourceTracker<
        std::shared_ptr<mce::ResourceBlockTemplate<dragon::ResolvedMaterialResource, mce::UncheckedHandleTracker, dragon::BufferDescription>>,
        std::weak_ptr<mce::ResourceBlockTemplate<dragon::ResolvedMaterialResource, mce::UncheckedHandleTracker, dragon::BufferDescription>>>;

    using ResourceServiceContext = ResourceServiceContextMember;

    FactoryTracker            mResourceTracker;        // off+0
    ResourceServiceContext    mResourceServiceContext; // off+48
    mce::ResourceServiceState mState;                  // off+120
};