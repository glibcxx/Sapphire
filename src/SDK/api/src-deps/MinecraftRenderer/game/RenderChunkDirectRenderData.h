#pragma once

#include "SDK/api/src-deps/Core/CheckedResourceService/ServerResourcePointer.h"
#include "SDK/api/src-deps/MinecraftRenderer/framebuilder/RenderMetadata.h"

namespace dragon {
    struct ResolvedVertexBufferResource;
    struct ResolvedIndexBufferResource;
} // namespace dragon

// size: 152 (1.21.50)
struct RenderChunkDirectVertexData {
    const std::optional<dragon::RenderMetadata>                                     mRenderMetadata;  // off+0
    bool                                                                            mAllowHalfFloats; // off+72
    std::optional<mce::ServerResourcePointer<dragon::ResolvedVertexBufferResource>> mVertexData;      // off+80
    std::optional<mce::ServerResourcePointer<dragon::ResolvedVertexBufferResource>> mUnkData;         // off+112
    size_t                                                                          mVertexCount;     // off+144
};

// size: 152 (1.21.50)
struct RenderChunkDirectIndexData {
    const std::optional<dragon::RenderMetadata>                                    mRenderMetadata;  // off+0
    bool                                                                           mAllowHalfFloats; // off+72
    std::optional<mce::ServerResourcePointer<dragon::ResolvedIndexBufferResource>> mVertexData;      // off+80
    std::optional<mce::ServerResourcePointer<dragon::ResolvedIndexBufferResource>> mUnkData;         // off+112
    size_t                                                                         mVertexCount;     // off+144
};