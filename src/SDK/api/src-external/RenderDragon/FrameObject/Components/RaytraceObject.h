#pragma once

#include "SDK/api/src-external/RenderDragon/Resource/DragonTextureResourceService.h"

namespace DFC /*dragon::frameobject::components*/ {

    namespace details {

        enum class RayTraceObjectType : uint8_t {};
        enum class RayTraceMaterialFlags : uint16_t {};

    } // namespace details

    // size: 128
    struct RaytraceObject {
        mce::ServerResourcePointer<void>    mBLASHandle;     // off+0
        DFC::details::RayTraceObjectType    mObjectType;     // off+24
        int64_t                             mUniqueId;       // off+32
        DFC::details::RayTraceMaterialFlags mMaterialFlags;  // off+40
        dragon::ServerTexture               mTextures[3];    // off+48
        uint32_t                            mTintColours[2]; // off+120
    };
    static_assert(sizeof(RaytraceObject) == 128);

} // namespace DFC