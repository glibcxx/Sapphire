#pragma once

#include <cstdint>
#include "SDK/api/src-external/bx/allocator.h"

namespace bgfx {

    // size: 1
    struct Fatal {
        enum class Enum : int {
            DebugCheck = 0,
            InvalidShader = 1,
            UnableToInitialize = 2,
            UnableToCreateTexture = 3,
            DeviceLost = 4,
            Count = 5,
        };
    };

    // size: 2
    struct VertexDeclHandle {
        uint16_t idx; // off+0
    };

    struct VertexBufferHandle {
        uint16_t idx; // off+0
    };

    // size: 2
    struct TextureHandle {
        uint16_t idx; // off+0
    };

    // size: 2
    struct FrameBufferHandle {
        uint16_t idx; // off+0
    };

    // size: 2
    struct IndexBufferHandle {
        uint16_t idx; // off+0
    };

    // size: 2
    struct UniformHandle {
        uint16_t idx; // off+0
    };

    // size: 2
    struct ShaderHandle {
        uint16_t idx; // off+0
    };

    // size: 2
    struct ShaderBufferHandle {
        uint16_t idx; // off+0
    };

    // size: 2
    struct ProgramHandle {
        uint16_t idx; // off+0
    };

    // size: 2
    struct OcclusionQueryHandle {
        uint16_t idx; // off+0
    };

    // size: 2
    struct AccelerationStructureHandle {
        uint16_t idx; // off+0
    };

    // size: 16
    struct Memory {
        uint8_t *data; // off+0
        uint32_t size; // off+8
    };

    // size: 16
    struct DLSSOptions {
        enum class Mode : int {
            DLSS = 0,
            Fallback = 1,
            Off = 2,
            Count = 3,
        };

        enum class Scaling : int {
            Performance = 0,
            Balanced = 1,
            Quality = 2,
            UltraPerformance = 3,
            UltraQuality = 4,
            Off = 5,
            Count = 6,
        };

        Mode    mMode;       // off+0
        Scaling mQuality;    // off+4
        float   mSharpness;  // off+8
        float   mMipmapBias; // off+12
    };

    // size: 1
    struct RendererType {
        enum class Enum : int {
            Noop = 0,
            Direct3D9 = 1,
            Direct3D11 = 2,
            Direct3D12 = 3,
            Direct3D12RTX = 4,
            Gnm = 5,
            Metal = 6,
            OpenGLES = 7,
            OpenGL = 8,
            Vulkan = 9,
            Nvn = 10,
            Count = 11,
        };
    };

    // size: 1
    struct TextureFormat {
        enum class Enum : int {
            BC1 = 0,
            BC2 = 1,
            BC3 = 2,
            BC4 = 3,
            BC5 = 4,
            BC6H = 5,
            BC7 = 6,
            ETC1 = 7,
            ETC2 = 8,
            ETC2A = 9,
            ETC2A1 = 10,
            PTC12 = 11,
            PTC14 = 12,
            PTC12A = 13,
            PTC14A = 14,
            PTC22 = 15,
            PTC24 = 16,
            ATC = 17,
            ATCE = 18,
            ATCI = 19,

            // ...
        };
    };

    // size: 1
    struct UniformType {
        enum class Enum : int {
            Sampler = 0,
            End = 1,
            Vec4 = 2,
            Mat3 = 3,
            Mat4 = 4,
            Count = 5,
        };
    };

    // size: 1
    struct AccelerationStructureBuildFlags {
        enum class Enum : uint16_t {
            NONE = 0,
            ALLOW_UPDATE = 1,
            ALLOW_COMPACTION = 2,
            PREFER_FAST_TRACE = 4,
            PREFER_FAST_BUILD = 8,
            MINIMIZE_MEMORY = 16,
            PERFORM_UPDATE = 32,
        };
    };

    // size: 12
    struct Resolution {
        uint32_t width;  // off+0
        uint32_t height; // off+4
        uint32_t reset;  // off+8
    };

    // size: 8
    struct CallbackI {
        virtual ~CallbackI() noexcept = 0;
    };

    // size: 56
    struct Init {
        bgfx::RendererType::Enum type;                   // off+0
        uint16_t                 vendorId;               // off+4
        uint16_t                 deviceId;               // off+6
        bool                     GPUBasedValidation : 1; // off+8
        bool                     mUnk8_1            : 1; // off+8
        bool                     debug              : 1; // off+8
        bool                     profile;                // off+9
        bgfx::Resolution         resolution;             // off+12

        // size: 16
        struct Limits {
            uint16_t maxEncoders;        // off+0
            uint32_t transientVbSize;    // off+4
            uint32_t transientIbSize;    // off+8
            uint8_t  maxRendererWorkers; // off+12
        };

        bgfx::Init::Limits limits;    // off+24
        bgfx::CallbackI   *callback;  // off+40
        bx::AllocatorI    *allocator; // off+48
    };

    // size: 80
    struct VertexDecl {
        uint32_t m_hash;           // off+0
        uint16_t m_stride;         // off+4
        uint16_t m_offset[18];     // off+6
        uint16_t m_attributes[18]; // off+42
    };

    // size: 6
    struct Attachment {
        bgfx::TextureHandle handle; // off+0
        uint16_t            mip;    // off+2
        uint16_t            layer;  // off+4
    };

} // namespace bgfx