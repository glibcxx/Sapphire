#pragma once

#include "SDK/api/src-deps/Core/Math/Math.h"
#include "SDK/api/src-deps/Core/CheckedResourceService/ServerResourcePointer.h"
#include "SDK/api/src-deps/MinecraftRenderer/renderer/Mesh.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonBufferResourceService.h"

namespace mce::framebuilder::bgfxbridge {

    // size: 24 (1.21.50)
    struct RayTraceableMeshMetadata {
        mce::ServerResourcePointer<mce::ResourcePointer<dragon::ResolvedAccelerationStructureResource>>
            mAccelerationStructureHandle; // off+0
    };

    // size: 568 (1.21.2), 592 (1.21.50/1.21.60)
    struct RayTraceableMesh {
        RayTraceableMeshMetadata mMetadata;                 // off+0
        mce::Mesh                mMesh;                     // off+24
        uint64_t                 mFrameNumLastTimeAccessed; // off+560 (1.21.2), 584 (1.21.50/1.21.60)
    };

    // size: 24
    struct RayTraceableMeshKey {
        const int64_t          mMeshGroupId; // off+0
        const uint16_t         mSubSpanId;   // off+8
        const mce::Mesh *const mMeshPtr;     // off+16
    };

    // size: 1
    struct RayTraceableMeshKeyHasher {
        std::size_t operator()(const RayTraceableMeshKey &_Keyval) const {
            return mce::Math::hash3(_Keyval.mMeshGroupId, _Keyval.mSubSpanId, _Keyval.mMeshPtr);
        }
    };

} // namespace mce::framebuilder::bgfxbridge