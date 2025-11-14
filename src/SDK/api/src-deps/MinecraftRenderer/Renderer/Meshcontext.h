#pragma once

#include <cstdint>
#include <optional>
#include <glm/glm.hpp>
#include "SDK/api/src-deps/Renderer/Color.h"
#include "SDK/api/src-deps/Core/CheckedResourceService/ServerResourcePointer.h"

namespace dragon {
    struct BufferDescription;
}

namespace mce {

    class RenderContext;
    class Camera;
    class GlobalConstantBuffers;
    class GlobalConstantBufferManager;
    class BufferResourceService;
    class QuadIndexBuffer;
    class ImmediateBuffer;
    class PerFrameHandleTracker;

    // size: 120
    class MeshContext {
    public:
        using SubClientId = uint8_t;

        mce::RenderContext                              &renderContext;          // off+0
        mce::Camera                                     &camera;                 // off+8
        mce::GlobalConstantBuffers                      &constantBuffers;        // off+16
        mce::GlobalConstantBufferManager                &constantBufferManager;  // off+24
        ShaderColor                                     &currentShaderColor;     // off+32
        ShaderColor                                     &currentShaderDarkColor; // off+40
        mce::BufferResourceService                      &bufferResourceService;  // off+48
        mce::QuadIndexBuffer                            &currentQuadIndexBuffer; // off+56
        mce::ServerResourcePointer<mce::ImmediateBuffer> immediateBuffer;        // off+64
        std::optional<glm::tvec4<float>>                 normalizedClipRegion;   // off+88

        SubClientId subClientId; // off+108

        /// vvv Order might be wrong vvv
        bool isDrawingUI;                 // off+109
        bool isDrawingFirstPersonObjects; // off+110
        bool isDrawingInLevelCubeMap;     // off+111
        bool isUsingVRPatch;              // off+112
        bool isDrawingEnvironmentalText;  // off+113
        bool mUnk114;                     // off+114
#if MC_VERSION == v1_21_60
        bool mUnk115; // off+115
        bool mUnk116; // off+116
#endif
    };
    static_assert(sizeof(MeshContext::immediateBuffer) == 24);
    static_assert(sizeof(MeshContext) == 120);

} // namespace mce