#pragma once

#include <gsl/gsl>
#include "SDK/api/src-deps/Core/CheckedResourceService/CheckedResourceService.h"
#include "SDK/api/src-deps/Core/CheckedResourceService/UncheckedHandleTracker.h"
#include "SDK/api/src-deps/Core/CheckedResourceService/AssertResourceServiceErrorHandler.h"
#include "SDK/api/src-deps/Core/CheckedResourceService/SimpleResourceTracker.h"
#include "SDK/api/src-deps/Core/CheckedResourceService/ClientResourcePointer.h"
#include "SDK/api/src-external/RenderDragon/Mesh/VertexBufferResourceManagerTypes.h"

namespace dragon {

    namespace mesh {
        class VertexDeclManager;
        struct VertexSpanRegistry;
        struct IndexSpanRegistry;
    } // namespace mesh
    namespace task {
        class GraphicsTasks;
    }

    struct DragonBufferResourceServiceContext {
        gsl::not_null<dragon::mesh::VertexDeclManager *>  mVertexDeclManager; // off+0
        gsl::not_null<dragon::mesh::VertexSpanRegistry *> mVertexSpanStorage; // off+8
        gsl::not_null<dragon::mesh::IndexSpanRegistry *>  mIndexSpanStorage;  // off+16
    };

    struct Unk1768ResourceServiceContext {
        dragon::task::GraphicsTasks *mGraphicsTasks; // off+0
    };

    struct ResolvedShaderBuffer {};

    struct ResolvedShaderBufferResource : ResolvedShaderBuffer {};

    struct ResolvedAccelerationStructure {};

    struct ResolvedAccelerationStructureResource : ResolvedAccelerationStructure {};

    struct Unk1768Resource {};

    using VertexBufferResourceService = mce::CheckedResourceService<dragon::ResolvedVertexBufferResource>;
    using IndexBufferResourceService = mce::CheckedResourceService<dragon::ResolvedIndexBufferResource>;
    using ShaderBufferResourceService = mce::CheckedResourceService<dragon::ResolvedShaderBufferResource>;
    using AccelerationStructureResourceService = mce::CheckedResourceService<dragon::ResolvedAccelerationStructureResource>;

    using Unk1768ResourceService = mce::CheckedResourceService<dragon::Unk1768Resource>;

}; // namespace dragon

// size: 80
template <>
class mce::CheckedResourceService<dragon::ResolvedVertexBufferResource> {
public:
    using ResourceServiceContextMember = dragon::DragonBufferResourceServiceContext;

    using FactoryTracker = mce::SimpleResourceTracker<
        std::shared_ptr<mce::ResourceBlockTemplate<dragon::ResolvedVertexBufferResource, mce::UncheckedHandleTracker, dragon::BufferDescription>>,
        std::weak_ptr<mce::ResourceBlockTemplate<dragon::ResolvedVertexBufferResource, mce::UncheckedHandleTracker, dragon::BufferDescription>>>;

    using ResourceServiceContext = ResourceServiceContextMember;

    FactoryTracker            mResourceTracker;        // off+0
    ResourceServiceContext    mResourceServiceContext; // off+48
    mce::ResourceServiceState mState;                  // off+72
};

// size: 80
template <>
class mce::CheckedResourceService<dragon::ResolvedIndexBufferResource> {
public:
    using ResourceServiceContextMember = dragon::DragonBufferResourceServiceContext;

    using FactoryTracker = mce::SimpleResourceTracker<
        std::shared_ptr<mce::ResourceBlockTemplate<dragon::ResolvedIndexBufferResource, mce::UncheckedHandleTracker, dragon::BufferDescription>>,
        std::weak_ptr<mce::ResourceBlockTemplate<dragon::ResolvedIndexBufferResource, mce::UncheckedHandleTracker, dragon::BufferDescription>>>;

    using ResourceServiceContext = ResourceServiceContextMember;

    FactoryTracker            mResourceTracker;        // off+0
    ResourceServiceContext    mResourceServiceContext; // off+48
    mce::ResourceServiceState mState;                  // off+72
};

// size: 80
template <>
class mce::CheckedResourceService<dragon::ResolvedShaderBufferResource> {
public:
    using ResourceServiceContextMember = dragon::DragonBufferResourceServiceContext;

    using FactoryTracker = mce::SimpleResourceTracker<
        std::shared_ptr<mce::ResourceBlockTemplate<dragon::ResolvedShaderBufferResource, mce::UncheckedHandleTracker, dragon::BufferDescription>>,
        std::weak_ptr<mce::ResourceBlockTemplate<dragon::ResolvedShaderBufferResource, mce::UncheckedHandleTracker, dragon::BufferDescription>>>;

    using ResourceServiceContext = ResourceServiceContextMember;

    FactoryTracker            mResourceTracker;        // off+0
    ResourceServiceContext    mResourceServiceContext; // off+48
    mce::ResourceServiceState mState;                  // off+72
};

// size: 80
template <>
class mce::CheckedResourceService<dragon::ResolvedAccelerationStructureResource> {
public:
    using ResourceServiceContextMember = dragon::DragonBufferResourceServiceContext;

    using FactoryTracker = mce::SimpleResourceTracker<
        std::shared_ptr<mce::ResourceBlockTemplate<dragon::ResolvedAccelerationStructureResource, mce::UncheckedHandleTracker, dragon::BufferDescription>>,
        std::weak_ptr<mce::ResourceBlockTemplate<dragon::ResolvedAccelerationStructureResource, mce::UncheckedHandleTracker, dragon::BufferDescription>>>;

    using ResourceServiceContext = ResourceServiceContextMember;

    FactoryTracker            mResourceTracker;        // off+0
    ResourceServiceContext    mResourceServiceContext; // off+48
    mce::ResourceServiceState mState;                  // off+72
};

// size: 80
template <>
class mce::CheckedResourceService<dragon::Unk1768Resource> {
public:
    using ResourceServiceContextMember = dragon::Unk1768ResourceServiceContext;

    using FactoryTracker = mce::SimpleResourceTracker<
        std::shared_ptr<mce::ResourceBlockTemplate<dragon::Unk1768Resource, mce::UncheckedHandleTracker, dragon::BufferDescription>>,
        std::weak_ptr<mce::ResourceBlockTemplate<dragon::Unk1768Resource, mce::UncheckedHandleTracker, dragon::BufferDescription>>>;

    using ResourceServiceContext = ResourceServiceContextMember;

    FactoryTracker            mResourceTracker;        // off+0
    ResourceServiceContext    mResourceServiceContext; // off+48
    mce::ResourceServiceState mState;                  // off+72
};