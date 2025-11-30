#pragma once

#include <functional>
#include "SDK/core/SymbolResolver.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonTextureDescription.h"
#include "SDK/api/src-deps/Core/CheckedResourceService/ClientResourcePointer.h"
#include "SDK/api/src-deps/Core/CheckedResourceService/CheckedResourceService.h"
#include "SDK/api/src-deps/Core/CheckedResourceService/ServerResourcePointer.h"
#include "SDK/api/src-deps/Core/CheckedResourceService/SimpleResourceTracker.h"
#include "SDK/api/src-deps/Core/CheckedResourceService/UncheckedHandleTracker.h"
#include "SDK/api/src-deps/Core/Threading/MPMCQueue.h"
#include "ResolvedTextureResource.h"
#include "DragonTextureTransaction.h"

namespace dragon {

    // size: 16
    struct TaskQueueContext {
        mpmc::Sender<std::function<void(void)>> queue; // off+0
    };

    // size: 24
    class ServerTexture : public mce::ServerResourcePointer<ResolvedTextureResource> {
    };

    // size: 24
    class ClientTexture : public mce::ClientResourcePointer<ResolvedTextureResource> {
    };

    // size: 72
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

    using ResourcePtr = mce::ResourcePointer<dragon::ResolvedTextureResource>;

    using ResourceServiceContext = ResourceServiceContextMember;

    FactoryTracker            mResourceTracker;        // off+0
    ResourceServiceContext    mResourceServiceContext; // off+48
    mce::ResourceServiceState mState;                  // off+64

    template <typename TransactionCtn, typename Payload>
    dragon::ClientTexture _create(Payload &payload);

    template <>
    SAPPHIRE_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x89\x7C\x24\x00\x48\x8B\x45")
    SAPPHIRE_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x0F\xBE\x45\x00\x4C\x8D\x3D") // LINE
        SDK_API dragon::ClientTexture _create<
            mce::TransactionContainer<dragon::res::CreateTextureTransaction, dragon::TextureResourceService>,
            dragon::res::CreateTextureTransaction>(dragon::res::CreateTextureTransaction &payload);

    template <>
    SAPPHIRE_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x48\x8B\x85\x00\x00\x00\x00\x48\x8B\x8D\x00\x00\x00\x00\x48\x89\x45")
    SAPPHIRE_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x8B\x9D\x00\x00\x00\x00\x48\x8B\xBD\x00\x00\x00\x00\x48\x89\x5C\x24") // LINE
        SDK_API dragon::ClientTexture _create<
            mce::TransactionContainer<dragon::res::CreateReadbackTextureTransaction, dragon::TextureResourceService>,
            dragon::res::CreateReadbackTextureTransaction>(dragon::res::CreateReadbackTextureTransaction &payload);
};
