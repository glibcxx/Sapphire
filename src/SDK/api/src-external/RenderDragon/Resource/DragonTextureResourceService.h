#pragma once

#include <functional>
#include "SDK/api/src-external/RenderDragon/Resource/DragonTextureDescription.h"
#include "SDK/api/src-deps/Core/CheckedResourceService/CheckedResourceService.h"
#include "SDK/api/src-deps/Core/CheckedResourceService/ServerResourcePointer.h"
#include "SDK/api/src-deps/Core/CheckedResourceService/SimpleResourceTracker.h"
#include "SDK/api/src-deps/Core/CheckedResourceService/UncheckedHandleTracker.h"
#include "SDK/api/src-deps/Core/Threading/MPMCQueue.h"

namespace dragon {

    struct ResolvedTextureResource {};

    struct TaskQueueContext {
        mpmc::Sender<std::function<void(void)>> queue; // off+0
    };

    class ServerTexture : public mce::ServerResourcePointer<ResolvedTextureResource> {
    };

    using TextureResourceService = mce::CheckedResourceService<ResolvedTextureResource>;

} // namespace dragon

// size: 72
template <>
class mce::CheckedResourceService<dragon::ResolvedTextureResource> {
public:
    using ResourceServiceContextMember = dragon::TaskQueueContext;

    using FactoryTracker = mce::SimpleResourceTracker<
        std::shared_ptr<mce::ResourceBlockTemplate<dragon::ResolvedTextureResource, mce::UncheckedHandleTracker, dragon::BufferDescription>>,
        std::weak_ptr<mce::ResourceBlockTemplate<dragon::ResolvedTextureResource, mce::UncheckedHandleTracker, dragon::BufferDescription>>>;

    using ResourceServiceContext = ResourceServiceContextMember;

    FactoryTracker            mResourceTracker;        // off+0
    ResourceServiceContext    mResourceServiceContext; // off+48
    mce::ResourceServiceState mState;                  // off+64
};