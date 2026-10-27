#pragma once

#include "pch.h" // IWYU pragma: keep

#include "SDK/api/src-external/bx/allocator.h"

namespace bgfx {

    struct Context;
    struct Init;
    struct PlatformData;
    struct RendererCreator;
    struct RendererContextI;
    struct Caps;

    using ViewId = uint16_t;

    constexpr uint16_t kInvalidHandle = UINT16_MAX;

    SPHR_DECL_API("1.21.50", "mov", "\x48\x8B\x15\x00\x00\x00\x00\x45\x33\xDB")
    extern SPHR_API bgfx::Context *s_ctx;
    SPHR_DECL_API("1.21.50,1.21.60", "disp:8,mov", "\x4C\x0F\x47\xAD\x00\x00\x00\x00\x48\x8B\x0D")
    extern SPHR_API bx::AllocatorI *g_allocator;

    extern SPHR_API bgfx::Caps g_caps;

    SPHR_DECL_API("1.21.50", "mov", "\x0F\x10\x1D\x00\x00\x00\x00\x0F\x11\x84\x24")
    extern SPHR_API bgfx::PlatformData g_platformData;

    SPHR_DECL_API("1.21.50", "lea", "\x4C\x8D\x35\x00\x00\x00\x00\x4D\x8B\xF8")
    extern SPHR_API bgfx::RendererCreator s_rendererCreator[];

    // size: 1
    struct Fatal {
        enum Enum : int {
            DebugCheck = 0,
            InvalidShader = 1,
            UnableToInitialize = 2,
            UnableToCreateTexture = 3,
            DeviceLost = 4,
            Count = 5,
        };
    };

    // size: 1
    struct ViewMode {
        enum Enum : int {
            Default = 0,
            Sequential = 1,
            DepthAscending = 2,
            DepthDescending = 3,
            Count = 4,
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
    struct DynamicVertexBufferHandle {
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
    struct IndirectBufferHandle {
        uint16_t idx; // off+0
    };

    // size: 2
    struct DynamicIndexBufferHandle {
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

    template <typename HandleType>
    concept is_bgfx_like_handle = requires(HandleType h) {
        { h.idx } -> std::convertible_to<uint16_t>;
    };

    template <is_bgfx_like_handle Handle>
    constexpr bool isValid(Handle h) { return h.idx != kInvalidHandle; }

    struct RendererCreator {
    public:
        bgfx::RendererContextI *(*createFn)(const bgfx::Init &); // off+0
        void (*destroyFn)();                                     // off+8
        const char *name;                                        // off+16
        __int64     unk24;                                       // off+24
        bool        supported;                                   // off+32
    };

    // size: 16
    struct Memory {
        uint8_t *data; // off+0
        uint32_t size; // off+8
    };

    using ReleaseFn = void (*)(void *_ptr, void *_userData);

    // size: 32
    struct MemoryRef {
        bgfx::Memory mem;       // off+0
        ReleaseFn    releaseFn; // off+16
        void        *userData;  // off+24
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

    // size: 664 (1.21.50)
    struct RayTracingConfiguration {
        alignas(8) uint8_t _fill[664];
    };

    struct ViewStats;
    struct EncoderStats;

    // FIXME
    struct Stats {
    public:
        __int64             cpuTimeFrame;
        __int64             cpuTimeBegin;
        __int64             cpuTimeEnd;
        __int64             cpuTimerFreq;
        __int64             gpuTimeBegin;
        __int64             gpuTimeEnd;
        __int64             gpuTimerFreq;
        __int64             waitRender;
        __int64             waitSubmit;
        unsigned            numDraw;
        unsigned            numCompute;
        unsigned            maxGpuLatency;
        uint16_t            numDynamicIndexBuffers;
        uint16_t            numDynamicVertexBuffers;
        uint16_t            numFrameBuffers;
        uint16_t            numIndexBuffers;
        uint16_t            numOcclusionQueries;
        uint16_t            numPrograms;
        uint16_t            numShaders;
        uint16_t            numTextures;
        uint16_t            numUniforms;
        uint16_t            numVertexBuffers;
        uint16_t            numVertexDecls;
        __int64             textureMemoryUsed;
        __int64             rtMemoryUsed;
        int                 transientVbUsed;
        int                 transientIbUsed;
        unsigned            numPrims[5];
        size_t              gpuMemoryMax;
        size_t              gpuMemoryUsed;
        uint16_t            width;
        uint16_t            height;
        uint16_t            textWidth;
        uint16_t            textHeight;
        uint16_t            numViews;
        bgfx::ViewStats    *viewStats;
        unsigned char       numEncoders;
        bgfx::EncoderStats *encoderStats;
    };

    // size: 1
    struct RendererType {
        // 1.21.50
        enum Enum : int {
            Noop = 0,
            Direct3D9 = 1,
            Direct3D11 = 2,
            Direct3D12 = 3,
            Direct3D12RTX = 4,
            Gnm = 5,
            Agc = 6,
            Metal = 7,
            OpenGLES = 8,
            OpenGL = 9,
            Vulkan = 10,
            Nvn = 11,
            Count = 12,
        };
    };

    // size: 1
    struct TextureFormat {
        enum Enum : int {
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
        enum Enum : int {
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
        enum Enum : uint16_t {
            NONE = 0,
            ALLOW_UPDATE = 1,
            ALLOW_COMPACTION = 2,
            PREFER_FAST_TRACE = 4,
            PREFER_FAST_BUILD = 8,
            MINIMIZE_MEMORY = 16,
            PERFORM_UPDATE = 32,
        };
    };

    // size: 1
    struct Access {
        enum Enum : int {
            Read = 0,
            Write = 1,
            ReadWrite = 2,
            Count = 3,
        };
    };

    // size: 1
    struct Attrib {
    public:
        // 1.21.50 (0x1465EC4F0)
        enum class Enum : int {
            Position = 0,
            Normal = 1,
            Tangent = 2,
            Bitangent = 3,
            Color0 = 4,
            Color1 = 5,
            Color2 = 6,
            Color3 = 7,
            Indices = 8,
            Weight = 9,
            TexCoord0 = 10,
            TexCoord1 = 11,
            TexCoord2 = 12,
            TexCoord3 = 13,
            TexCoord4 = 14,
            TexCoord5 = 15,
            TexCoord6 = 16,
            TexCoord7 = 17,
            TexCoord8 = 18,
            Count = 19,
        };
    };

    // size: 1
    struct AttribType {
    public:
        enum class Enum : int {
            Uint8 = 0,
            Int8 = 1,
            Uint10 = 2,
            Uint16 = 3,
            Int16 = 4,
            Half = 5,
            Float = 6,
            Count = 7,
        };
    };

    // size: 8
    struct AttribToId {
    public:
        bgfx::Attrib::Enum attr; // off+0
        uint16_t           id;   // off+4
    };

    // size: 8
    struct AttribTypeToId {
    public:
        bgfx::AttribType::Enum type; // off+0
        uint16_t               id;   // off+4
    };

    constexpr bgfx::AttribToId s_attribToId[19] = {
        bgfx::AttribToId{bgfx::Attrib::Enum::Position,  1   },
        bgfx::AttribToId{bgfx::Attrib::Enum::Normal,    2   },
        bgfx::AttribToId{bgfx::Attrib::Enum::Tangent,   3   },
        bgfx::AttribToId{bgfx::Attrib::Enum::Bitangent, 4   },
        bgfx::AttribToId{bgfx::Attrib::Enum::Color0,    5   },
        bgfx::AttribToId{bgfx::Attrib::Enum::Color1,    6   },
        bgfx::AttribToId{bgfx::Attrib::Enum::Color2,    0x18},
        bgfx::AttribToId{bgfx::Attrib::Enum::Color3,    0x19},
        bgfx::AttribToId{bgfx::Attrib::Enum::Indices,   0x0E},
        bgfx::AttribToId{bgfx::Attrib::Enum::Weight,    0x0F},
        bgfx::AttribToId{bgfx::Attrib::Enum::TexCoord0, 0x10},
        bgfx::AttribToId{bgfx::Attrib::Enum::TexCoord1, 0x11},
        bgfx::AttribToId{bgfx::Attrib::Enum::TexCoord2, 0x12},
        bgfx::AttribToId{bgfx::Attrib::Enum::TexCoord3, 0x13},
        bgfx::AttribToId{bgfx::Attrib::Enum::TexCoord4, 0x14},
        bgfx::AttribToId{bgfx::Attrib::Enum::TexCoord5, 0x15},
        bgfx::AttribToId{bgfx::Attrib::Enum::TexCoord6, 0x16},
        bgfx::AttribToId{bgfx::Attrib::Enum::TexCoord7, 0x17},
        bgfx::AttribToId{bgfx::Attrib::Enum::TexCoord8, 0x1A},
    };

    // size: 24
    struct InstanceDataBuffer {
        uint8_t                 *data;   // off+0
        uint32_t                 size;   // off+8
        uint32_t                 offset; // off+12
        uint32_t                 num;    // off+16
        uint16_t                 stride; // off+20
        bgfx::VertexBufferHandle handle; // off+22
    };

    // size: 24
    struct TransientIndexBuffer {
    public:
        unsigned char          *data;       // off+0
        unsigned                size;       // off+8
        unsigned                startIndex; // off+12
        bgfx::IndexBufferHandle handle;     // off+16
    };

    // size: 24
    struct TransientVertexBuffer {
    public:
        unsigned char           *data;        // off+0
        unsigned                 size;        // off+8
        unsigned                 startVertex; // off+12
        uint16_t                 stride;      // off+16
        bgfx::VertexBufferHandle handle;      // off+18
        bgfx::VertexDeclHandle   decl;        // off+20
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

    // size: 64 (1.21.50)
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
        uint64_t           mUnk56;    // off+56
    };

    constexpr uint8_t s_attribTypeSizeD3D9[7][4] = {
        {4, 4, 4,  4 },
        {4, 4, 4,  4 },
        {4, 4, 4,  4 },
        {4, 4, 8,  8 },
        {4, 4, 8,  8 },
        {4, 4, 8,  8 },
        {4, 8, 12, 16}
    };

    constexpr uint8_t s_attribTypeSizeD3D1x[7][4] = {
        {1, 2, 4,  4 },
        {1, 2, 4,  4 },
        {4, 4, 4,  4 },
        {2, 4, 8,  8 },
        {2, 4, 8,  8 },
        {2, 4, 8,  8 },
        {4, 8, 12, 16}
    };
    constexpr uint8_t s_attribTypeSizeGl[7][4] = {
        {1, 2, 4,  4 },
        {1, 2, 4,  4 },
        {4, 4, 4,  4 },
        {2, 4, 6,  8 },
        {2, 4, 6,  8 },
        {2, 4, 6,  8 },
        {4, 8, 12, 16}
    };
    constexpr uint8_t s_attribTypeSizeNvn[7][4] = {
        {1, 2, 4,  4 },
        {1, 2, 4,  4 },
        {4, 4, 4,  4 },
        {2, 4, 8,  8 },
        {2, 4, 8,  8 },
        {2, 4, 8,  8 },
        {4, 8, 12, 16}
    };

    constexpr const uint8_t (*s_attribTypeSize[12])[7][4] = {
        &s_attribTypeSizeD3D9,
        &s_attribTypeSizeD3D9,
        &s_attribTypeSizeD3D1x,
        &s_attribTypeSizeD3D1x,
        &s_attribTypeSizeD3D1x,
        &s_attribTypeSizeGl,
        &s_attribTypeSizeGl,
        &s_attribTypeSizeGl,
        &s_attribTypeSizeD3D1x,
        &s_attribTypeSizeNvn,
        &s_attribTypeSizeD3D9,
    };

    constexpr uint8_t s_attribTypeIsInt[(size_t)AttribType::Enum::Count] = {1, 1, 1, 1, 1, 0, 0};

    // size: 84 (1.21.50)
    struct VertexDecl {
        uint32_t m_hash;           // off+0
        uint16_t m_stride = 0;     // off+4
        uint16_t m_offset[19];     // off+6
        uint16_t m_attributes[19]; // off+44

        VertexDecl() : m_hash(0), m_stride(0) {
            std::memset(m_offset, 0, sizeof(m_offset));
            std::memset(m_attributes, 0xFF, sizeof(m_attributes));
        }

        bgfx::VertexDecl &begin(RendererType::Enum _renderer = RendererType::Noop) {
            m_hash = _renderer;
            m_stride = 0;
            std::memset(m_attributes, 0xFF, sizeof(m_attributes));
            std::memset(m_offset, 0, sizeof(m_offset));
            return *this;
        }

        bgfx::VertexDecl &add(
            bgfx::Attrib::Enum     _attrib,
            uint8_t                _num,
            bgfx::AttribType::Enum _type,
            bool                   _normalized = false,
            bool                   _asInt = false
        ) {
            const uint16_t encodedNorm = (_normalized & 1) << 7;
            const uint16_t encodedType = ((int)_type & 7) << 3;
            const uint16_t encodedNum = (_num - 1) & 3;
            const uint16_t encodeAsInt = ((uint8_t)_asInt & s_attribTypeIsInt[(int)_type]) << 8;

            m_attributes[(int)_attrib] = encodedNum | encodedType | encodedNorm | encodeAsInt;
            m_offset[(int)_attrib] = m_stride;
            m_stride += (*s_attribTypeSize[m_hash])[(int)_type][_num - 1];

            return *this;
        }

        VertexDecl &skip(uint8_t _num) {
            m_stride += _num;
            return *this;
        }

        SPHR_DECL_API("1.21.50", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x55\x48\x8B\xEC\x48\x83\xEC\x00\x45\x33\xC9") // 1.21.50
        SDK_API void end();

        void decode(
            bgfx::Attrib::Enum      _attrib,
            uint8_t                &_num,
            bgfx::AttribType::Enum &_type,
            bool                   &_normalized,
            bool                   &_asInt
        ) const {
            const uint16_t val = m_attributes[(int)_attrib];
            _num = static_cast<uint8_t>((val & 3) + 1);
            _type = static_cast<bgfx::AttribType::Enum>((val >> 3) & 7);
            _normalized = (val & 0x0080) != 0;
            _asInt = (val & 0x0100) != 0;
        }

        bool has(bgfx::Attrib::Enum _attrib) const { return m_attributes[(int)_attrib] != 0xFFFF; }

        uint16_t getOffset(bgfx::Attrib::Enum _attrib) const { return m_offset[(int)_attrib]; }

        uint16_t getStride() const { return m_stride; }

        uint32_t getSize(uint32_t _num) const { return _num * m_stride; }
    };

    // size: 6
    struct Attachment {
        bgfx::TextureHandle handle; // off+0
        uint16_t            mip;    // off+2
        uint16_t            layer;  // off+4
    };

    // size: 5752 (1.21.50)
    struct alignas(8) Caps {
        bgfx::RendererType::Enum rendererType;           // off+0
        char                     rendererVersion[32];    // off+4
        char                     adapterVersion[512];    // off+36
        uint64_t                 rendererFeatureVersion; // off+552
        uint64_t                 shaderModelVersion;     // off+560
        uint64_t                 supported;              // off+568
        uint16_t                 vendorId;               // off+576
        uint16_t                 deviceId;               // off+578
        unsigned                 subSysId;               // off+580
        unsigned                 revision;               // off+584
        size_t                   dedicatedVideoMemory;   // off+592
        size_t                   dedicatedSystemMemory;  // off+600
        size_t                   sharedSystemMemory;     // off+608
        size_t                   reservedVideoMemory;    // off+616
        char                     chipsetName[512];       // off+624
        bool                     homogeneousDepth;       // off+1136
        bool                     originBottomLeft;       // off+1137
        unsigned char            numGPUs;                // off+1138
        unsigned                 displayWidth;           // off+1140
        unsigned                 displayHeight;          // off+1144
        unsigned                 mUnk1148;

        // size: 1072 (1.21.50)
        struct GPU {
        public:
            uint16_t vendorId;              // off+0
            uint16_t deviceId;              // off+2
            unsigned subSysId;              // off+4
            unsigned revision;              // off+8
            size_t   dedicatedVideoMemory;  // off+16
            size_t   dedicatedSystemMemory; // off+24
            size_t   sharedSystemMemory;    // off+32
            size_t   reservedVideoMemory;   // off+40
            char     chipsetName[512];      // off+48
            char     adapterVersion[512];   // off+560
        };

        bgfx::Caps::GPU gpu[4]; // off+1152

        // size: 108
        struct Limits {
        public:
            unsigned maxDrawCalls;       // off+0
            unsigned maxBlits;           // off+4
            unsigned maxTextureSize;     // off+8
            unsigned maxTextureLayers;   // off+12
            unsigned maxViews;           // off+16
            unsigned maxFrameBuffers;    // off+20
            unsigned maxFBAttachments;   // off+24
            unsigned maxPrograms;        // off+28
            unsigned maxShaders;         // off+32
            unsigned maxTextures;        // off+36
            unsigned maxTextureSamplers; // off+40
            unsigned mUnk44;
            unsigned maxVertexDecls;   // off+48
            unsigned maxVertexStreams; // off+52
            unsigned mUnk56;
            unsigned maxIndexBuffers;              // off+60
            unsigned maxVertexBuffers;             // off+64
            unsigned maxDynamicIndexBuffers;       // off+68
            unsigned maxDynamicVertexBuffers;      // off+72
            unsigned maxUniforms;                  // off+76
            unsigned maxOcclusionQueries;          // off+80
            unsigned maxEncoders;                  // off+84
            unsigned transientVbSize;              // off+88
            unsigned transientIbSize;              // off+92
            unsigned maxAccelerationStructures;    // off+96
            unsigned avgAccelerationStructureSize; // off+100
            unsigned mUnk104;
        };

        bgfx::Caps::Limits limits;      // off+5440
        uint16_t           formats[93]; // off+5548
        const char        *mUnk5736;
        const char        *mUnk5744;
    };

    // size: 1
    struct Encoder {
        SPHR_DECL_API("1.21.50", "\x48\x89\x5C\x24\x00\x48\x89\x7C\x24\x00\x4C\x8B\x11\x0F\xB7\xFA") // 1.21.50
        SDK_API uint16_t setScissor(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height);

        SPHR_DECL_API("1.21.50", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x41\x0F\xB7\xC0") // 1.21.50
        SDK_API void setVertexBuffer(
            uint8_t                   _stream,
            DynamicVertexBufferHandle _handle,
            uint32_t                  _startVertex,
            uint32_t                  _numVertices,
            VertexDeclHandle          _decl,
            uint32_t                  _stride
        );
    };

    SPHR_DECL_API("1.21.50", "\x40\x53\x56\x57\x48\x83\xEC\x00\x0F\xB6\xF2") // 1.21.50
    SDK_API int init(const Init &_init, bool a2);

    SPHR_DECL_API("1.21.50", "\x48\x89\x5C\x24\x00\x55\x56\x57\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x48\x8B\xFA\x48\x8B\xD9\x48\x83\x3D") // 1.21.50
    SDK_API int initCaps(const Init &_init, int &a2);

    inline const bgfx::Memory *alloc(uint32_t _size) {
        Memory *mem = (Memory *)g_allocator->realloc(nullptr, sizeof(Memory) + _size);
        mem->size = _size;
        mem->data = (uint8_t *)mem + sizeof(Memory);
        return mem;
    }

    inline const bgfx::Memory *copy(const void *_data, uint32_t _size) {
        const Memory *mem = alloc(_size);
        ::memcpy(mem->data, _data, _size);
        return mem;
    }

    inline const bgfx::Memory *makeRef(
        const void *_data, uint32_t _size, ReleaseFn _releaseFn = nullptr, void *_userData = nullptr
    ) {
        MemoryRef *memRef = (MemoryRef *)g_allocator->realloc(g_allocator, sizeof(MemoryRef));
        memRef->mem.size = _size;
        memRef->mem.data = (uint8_t *)_data;
        memRef->releaseFn = _releaseFn;
        memRef->userData = _userData;
        return &memRef->mem;
    }

    SPHR_DECL_API("1.21.50", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x41\x0F\xB7\xF8") // 1.21.50
    SDK_API bgfx::DynamicVertexBufferHandle createDynamicVertexBuffer(
        const bgfx::Memory *_mem, const bgfx::VertexDecl &_decl, uint16_t _flags = 0x0000
    );

    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x56\x41\x57\x48\x83\xEC\x00\x0F\xB7\xFA") // 1.21.50
    SDK_API bgfx::DynamicIndexBufferHandle createDynamicIndexBuffer(const bgfx::Memory *_mem, uint16_t _flags = 0x0000);

    SPHR_DECL_API("1.21.50", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x48\x8B\x35\x00\x00\x00\x00\x4C\x8B\xF1") // 1.21.50
    SDK_API void allocInstanceDataBuffer(InstanceDataBuffer *_idb, uint32_t _num, uint16_t _stride);

    SPHR_DECL_API("1.21.50", "\x40\x53\x55\x56\x57\x41\x56\x41\x57\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x48\x8B\x2D") // 1.21.50
    SDK_API void updateOffset(
        bgfx::DynamicVertexBufferHandle _handle, uint32_t _offset, uint32_t _declStride, const bgfx::Memory *_mem
    );

} // namespace bgfx