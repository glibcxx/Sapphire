#pragma once

#include "pch.h" // IWYU pragma: keep

#include <d3d12.h>
#include "bgfx.h"
#include "SDK/api/src-external/bgfx/defines.h"
#include "SDK/api/src-external/bx/handlealloc.h"
#include "SDK/api/src-external/bx/float4x4_t.h"
#include "SDK/api/src-external/bx/thread.h"
#include "SDK/api/src-external/bx/semaphore.h"
#include "SDK/api/src-external/bx/mutex.h"
#include "config.h"
#include "platform.h"
#include "common/Memory.hpp"

#define BGFX_SUBMIT_INTERNAL_NONE UINT8_C(0x00)
#define BGFX_SUBMIT_INTERNAL_INDEX32 UINT8_C(0x40)
#define BGFX_SUBMIT_INTERNAL_OCCLUSION_VISIBLE UINT8_C(0x80)
#define BGFX_SUBMIT_INTERNAL_RESERVED_MASK UINT8_C(0xff)

namespace bgfx {

    struct Frame;

    struct TransientVertexBuffer;
    struct TransientIndexBuffer;

    // size: 112 (1.21.50)
    struct ClearQuad {
    public:
        bgfx::TransientVertexBuffer *m_vb;         // off+0
        bgfx::VertexDecl             m_decl;       // off+8
        bgfx::ProgramHandle          m_program[8]; // off+92

        SPHR_DECL_API("1.21.50", "disp:297,call", "\x48\x0B\xD1\x48\x09\x15")
        SDK_API void init();
    };

    // size: 112 (1.21.50)
    struct TextVideoMemBlitter {
    public:
        bgfx::TextureHandle          m_texture; // off+0
        bgfx::TransientVertexBuffer *m_vb;      // off+8
        bgfx::TransientIndexBuffer  *m_ib;      // off+16
        bgfx::VertexDecl             m_decl;    // off+24
        bgfx::ProgramHandle          m_program; // off+108

        SPHR_DECL_API("1.21.50", "disp:17,call", "\x48\x0B\xD1\x48\x09\x15")
        SDK_API void init();
    };

    struct TopLevelInstanceDesc;

    // size: 262152
    class CommandBuffer {
    public:
        enum class Enum : int {
            RendererInit = 0,
            RendererShutdownBegin = 1,
            CreateVertexDecl = 2,
            CreateIndexBuffer = 3,
            CreateVertexBuffer = 4,
            CreateDynamicIndexBuffer = 5,
            UpdateDynamicIndexBuffer = 6,
            CreateDynamicVertexBuffer = 7,
            UpdateDynamicVertexBuffer = 8,
            UnkEnum9 = 9,
            CreateBottomLevelAccelerationStructure = 10,
            CreateTopLevelAccelerationStructure = 11,
            CreateShader = 12,
            CreateProgram = 13,
            CreateTexture = 14,
            CreateShaderBuffer = 15,
            UpdateTexture = 16,
            ResizeTexture = 17,
            WrapExternalTexture = 18,
            UpdateShaderBuffer = 19,
            CreateFrameBuffer = 20,
            CreateUniform = 21,
            UpdateViewName = 22,
            InvalidateOcclusionQuery = 23,
            SetName = 24,
            End = 25,
            RendererShutdownEnd = 26,
            DestroyVertexDecl = 27,
            DestroyIndexBuffer = 28,
            DestroyVertexBuffer = 29,
            DestroyDynamicIndexBuffer = 30,
            DestroyDynamicVertexBuffer = 31,
            DestroyShader = 32,
            DestroyProgram = 33,
            DestroyTexture = 34,
            DestroyShaderBuffer = 35,
            DestroyFrameBuffer = 36,
            DestroyUniform = 37,
            DestroyAccelerationStructure = 38,
            ReadTexture = 39,
            RequestScreenShot = 40,
        };

        CommandBuffer() = delete;

        void write(const void *_data, uint32_t _size) {
            ::memcpy(&m_buffer[m_pos], _data, _size);
            m_pos += _size;
        }

        template <typename Type>
        void write(const Type &_in) {
            align(alignof(Type));
            write(reinterpret_cast<const uint8_t *>(&_in), sizeof(Type));
        }

        void read(void *_data, uint32_t _size) {
            ::memcpy(_data, &m_buffer[m_pos], _size);
            m_pos += _size;
        }

        template <typename Type>
        void read(Type &_in) {
            align(alignof(Type));
            read(reinterpret_cast<uint8_t *>(&_in), sizeof(Type));
        }

        void align(uint32_t _alignment) {
            const uint32_t mask = _alignment - 1;
            const uint32_t pos = (m_pos + mask) & (~mask);
            m_pos = pos;
        }

        void reset() {
            m_pos = 0;
        }

        void start() {
            m_pos = 0;
            m_size = 0;
        }

        uint32_t m_pos;            // off+0
        uint32_t m_size;           // off+4
        uint8_t  m_buffer[262144]; // off+8
    };

    // size: 2
    struct UniformRegInfo {
        bgfx::UniformHandle m_handle; // off+0
    };

    // size: 24
    struct TextVideoMem {
        // size: 2
        struct MemSlot {
            uint8_t attribute; // off+0
            uint8_t character; // off+1
        };

        MemSlot *m_mem;    // off+0
        uint32_t m_size;   // off+8
        uint16_t m_width;  // off+12
        uint16_t m_height; // off+14
        bool     m_small;  // off+16
    };

    // size: 8
    struct PredefinedUniform {
        enum class Enum : int {
            ViewRect = 0,
            ViewTexel = 1,
            View = 2,
            InvView = 3,
            Proj = 4,
            InvProj = 5,
            ViewProj = 6,
            InvViewProj = 7,
            Model = 8,
            ModelView = 9,
            ModelViewProj = 10,
            AlphaRef = 11,
            Count = 12,
        };

        uint32_t m_loc;   // off+0
        uint16_t m_count; // off+4
        uint8_t  m_type;  // off+6
    };

    // size: 8
    struct Rect {
        uint16_t m_x;      // off+0
        uint16_t m_y;      // off+2
        uint16_t m_width;  // off+4
        uint16_t m_height; // off+6
    };

    // size: 64
    struct Matrix4 {
        // size: 64
        union {
            float          val[16]; // off+0
            bx::float4x4_t f4x4;    // off+0
        } un;                       // off+0
    };

    // size: 4
    struct Handle {
        enum class Enum : int {
            Shader = 0,
            Texture = 1,
            Count = 2,
        };
        uint16_t type; // off+0
        uint16_t idx;  // off+2
    };

    // size: 16
    struct SortKey {
        enum class Enum : int {
            SortProgram = 0,
            SortDepth = 1,
            SortSequence = 2,
        };

        uint32_t m_depth;   // off+0
        uint32_t m_seq;     // off+4
        uint16_t m_program; // off+8
        ViewId   m_view;    // off+10
        uint8_t  m_trans;   // off+12
    };

    // size: 4
    struct IndexBuffer {
        uint32_t m_size; // off+0
    };

    // size: 8
    struct VertexBuffer {
        uint32_t m_size;   // off+0
        uint16_t m_stride; // off+4
    };

    // size: 20
    struct DynamicIndexBuffer {
        bgfx::IndexBufferHandle m_handle;     // off+0
        uint32_t                m_offset;     // off+4
        uint32_t                m_size;       // off+8
        uint32_t                m_startIndex; // off+12
        uint16_t                m_flags;      // off+16
    };

    // size: 28
    struct DynamicVertexBuffer {
        bgfx::VertexBufferHandle m_handle;      // off+0
        uint32_t                 m_offset;      // off+4
        uint32_t                 m_size;        // off+8
        uint32_t                 m_startVertex; // off+12
        uint32_t                 m_numVertices; // off+16
        uint16_t                 m_stride;      // off+20
        bgfx::VertexDeclHandle   m_decl;        // off+22
        uint16_t                 m_flags;       // off+24
    };

    // size: 8
    struct Stream {
        uint32_t                 m_startVertex; // off+0
        bgfx::VertexBufferHandle m_handle;      // off+4
        bgfx::VertexDeclHandle   m_decl;        // off+6
    };

    // size: 20
    struct Binding {
        enum class Enum : int {
            Image = 0,
            IndexBuffer = 1,
            VertexBuffer = 2,
            Texture = 3,
            ShaderBuffer = 4,
            Count = 5,
        };

        uint32_t mUnk0; // off+0
        uint32_t mUnk4; // off+4
        uint32_t mUnk8; // off+8
        uint16_t m_idx; // off+12

        // size: 4
        union {
            // size: 4
            struct {
                uint32_t m_textureFlags; // off+0
            } m_draw;

            // size: 3
            struct {
                uint8_t m_format; // off+0
                uint8_t m_access; // off+1
                uint8_t m_mip;    // off+2
                uint8_t m_type;   // off+3
            } m_compute;
        } m_un; // off+8
    };

    // size: 1344
    struct alignas(64) RenderBind {
        bgfx::Binding m_bind[65]; // off+0
    };

    struct NativeCommandListScope;

    // size: 16
    struct Clear {
    public:
        uint8_t  m_index[8]; // @off: 0x0, @size: 0x8
        float    m_depth;    // @off: 0x8, @size: 0x4
        uint8_t  m_stencil;  // @off: 0xC, @size: 0x1
        uint16_t m_flags;    // @off: 0xE, @size: 0x2
    };

    struct alignas(64) View {
        bgfx::Clear             m_clear;   // @off: 0x0, @size: 0x10
        bgfx::Rect              m_rect;    // @off: 0x10, @size: 0x8
        bgfx::Rect              m_scissor; // @off: 0x18, @size: 0x8
        bgfx::Matrix4           m_view;    // @off: 0x20, @size: 0x40
        bgfx::Matrix4           m_proj[2]; // @off: 0x60, @size: 0x80
        bgfx::Matrix4           m_prevProj[2];
        float                   m_prevWorldPosOffset[4];
        bgfx::FrameBufferHandle m_fbh;   // @off: 0xE0, @size: 0x2
        uint8_t                 m_mode;  // @off: 0xE2, @size: 0x1
        uint8_t                 m_flags; // @off: 0xE3, @size: 0x1
        void (*m_nativeCallback)(void *, ::bgfx::NativeCommandListScope *);
        void *m_userData;
    };

    // size: 4194320
    struct MatrixCache {
    public:
        bgfx::Matrix4 m_cache[65536]; // off+0
        uint32_t      m_num;          // off+4194304
    };

    // size: 524292
    struct RectCache {
    public:
        bgfx::Rect m_cache[65536]; // off+0
        uint32_t   m_num;          // off+524288
    };

    // size: 4718624
    struct FrameCache {
    public:
        bgfx::MatrixCache m_matrixCache; // off+0
        bgfx::RectCache   m_rectCache;   // off+4194320
    };

    // size: 268435464
    class UniformBuffer {
    public:
        unsigned m_size;               // off+0
        unsigned m_pos;                // off+4
        char     m_buffer[0x10000000]; // off+8
    };

    // size: 128
    struct alignas(64) RenderDraw {
        bgfx::Stream               m_stream[4];              // off+0
        uint64_t                   m_stateFlags;             // off+32
        uint64_t                   m_stencil;                // off+40
        uint32_t                   m_rgba;                   // off+48
        uint32_t                   m_uniformBegin;           // off+52
        uint32_t                   m_uniformEnd;             // off+56
        uint32_t                   m_startMatrix = 0;        // off+60
        uint32_t                   m_startIndex = 0;         // off+64
        uint32_t                   m_numIndices = -1;        // off+68
        uint32_t                   m_indexSize = 0;          // off+72
        uint32_t                   m_numVertices = -1;       // off+76
        uint32_t                   m_instanceDataOffset = 0; // off+80
        uint32_t                   m_numInstances = 1;       // off+84
        uint16_t                   m_instanceDataStride = 0; // off+88
        uint16_t                   m_startIndirect = 0;      // off+90
        uint16_t                   m_numIndirect = -1;       // off+92
        uint16_t                   m_numMatrices = 1;        // off+94
        uint16_t                   m_scissor = -1;           // off+96
        uint8_t                    m_submitFlags = 1;        // off+98
        uint8_t                    m_streamMask;             // off+99
        uint8_t                    m_uniformIdx;             // off+100
        int32_t                    m_biasConst;              // off+104
        float                      m_biasSlope;              // off+108
        float                      m_biasClamp;              // off+112
        bgfx::IndexBufferHandle    m_indexBuffer;            // off+116
        bgfx::VertexBufferHandle   m_instanceDataBuffer;     // off+118
        bgfx::IndirectBufferHandle m_indirectBuffer;         // off+120
        bgfx::OcclusionQueryHandle m_occlusionQuery;         // off+122

        bool setStreamBit(uint8_t _stream, VertexBufferHandle _handle) {
            const uint8_t bit = 1 << _stream;
            const uint8_t mask = m_streamMask & ~bit;
            const uint8_t tmp = isValid(_handle) ? bit : 0;
            m_streamMask = mask | tmp;
            return 0 != tmp;
        }
    };

    // size: 64
    struct alignas(64) RenderCompute {
        uint32_t                   m_uniformBegin;   // off+0
        uint32_t                   m_uniformEnd;     // off+4
        uint32_t                   m_startMatrix;    // off+8
        bgfx::IndirectBufferHandle m_indirectBuffer; // off+12
        uint32_t                   m_numX;           // off+16
        uint32_t                   m_numY;           // off+20
        uint32_t                   m_numZ;           // off+24
        uint16_t                   m_startIndirect;  // off+28
        uint16_t                   m_numIndirect;    // off+30
        uint16_t                   m_numMatrices;    // off+32
        uint8_t                    m_submitFlags;    // off+34
        uint8_t                    m_uniformIdx;     // off+35
        bool                       m_isRayTracing;   // off+36
    };

    // size: 128
    union RenderItem {
    public:
        bgfx::RenderDraw    draw;
        bgfx::RenderCompute compute;
    };

    // FIXME
    struct ViewStats {
    public:
        char     name[256];
        uint16_t view;
        __int64  cpuTimeElapsed;
        __int64  gpuTimeElapsed;
    };

    // size: 64
    struct alignas(64) BlitItem {
    public:
        uint16_t            m_srcX;
        uint16_t            m_srcY;
        uint16_t            m_srcZ;
        uint16_t            m_dstX;
        uint16_t            m_dstY;
        uint16_t            m_dstZ;
        uint16_t            m_width;
        uint16_t            m_height;
        uint16_t            m_depth;
        uint8_t             m_srcMip;
        uint8_t             m_dstMip;
        bgfx::TextureHandle m_src;
        bgfx::TextureHandle m_dst;
    };

    // size: 103264576 (1.21.50)
    struct Frame {
        template <typename T, std::size_t Size>
        struct FreeHandle {
        public:
            T        m_queue[Size];
            uint16_t m_num;
        };

        template <typename T, std::size_t Size>
        struct PagedFreeHandle {
        public:
            struct Page {
                T     m_data[Size];
                Page *m_next;
            };

            struct ListNode {
                Page  *m_head;
                Page  *m_tail;
                size_t m_allocatedBytes;
            };

            T         m_inlineBuffer[Size];
            ListNode *m_overflowList;
            size_t    m_sizeInBytes;
            size_t    m_capacityInBytes;
            bool      m_fixedInlineOnly;
        };

        uint16_t                     m_viewRemap[256];        // off+0
        float                        m_colorPalette[16][4];   // off+512
        bgfx::View                   m_view[256];             // off+768
        int                          m_occlusion[256];        // off+115456
        uint64_t                     m_sortKeys[65536];       // off+116480
        uint16_t                     m_sortValues[65536];     // off+640768
        bgfx::RenderItem             m_renderItem[65536];     // off+771840
        bgfx::RenderBind             m_renderItemBind[65536]; // off+9160448
        __int64                      mUnk[65536];             // off+97240832
        uint32_t                     m_blitKeys[1025];        // off+97765120
        bgfx::BlitItem               m_blitItem[1025];        // off+97769280
        bgfx::FrameCache             m_frameCache;            // off+97834880
        bgfx::UniformBuffer        **m_uniformBuffer;         // off+102553504
        uint32_t                     m_numRenderItems;        // off+102553512
        uint16_t                     m_numBlitItems;          // off+102553516
        uint32_t                     m_iboffset;              // off+102553520
        uint32_t                     m_vboffset;              // off+102553524
        bgfx::TransientIndexBuffer  *m_transientIb;           // off+102553528
        bgfx::TransientVertexBuffer *m_transientVb;           // off+102553536
        bgfx::Resolution             m_resolution;            // off+102553544
        uint32_t                     m_debug;                 // off+102553556
        bgfx::CommandBuffer          m_cmdPre;                // off+102553560
        bgfx::CommandBuffer          m_cmdPost;               // off+102815712

        bgfx::Frame::FreeHandle<bgfx::IndexBufferHandle, 4096>            m_freeIndexBuffer;           // off+103077864
        bgfx::Frame::FreeHandle<bgfx::VertexDeclHandle, 64>               m_freeVertexDecl;            // off+103086058
        bgfx::Frame::FreeHandle<bgfx::VertexBufferHandle, 4096>           m_freeVertexBuffer;          // off+103086188
        bgfx::Frame::FreeHandle<bgfx::ShaderHandle, 4096>                 m_freeShader;                // off+103094382
        bgfx::Frame::FreeHandle<bgfx::ProgramHandle, 2048>                m_freeProgram;               // off+103102576
        bgfx::Frame::PagedFreeHandle<bgfx::TextureHandle, 4096>           m_freeTexture;               // off+103106680
        bgfx::Frame::FreeHandle<bgfx::ShaderBufferHandle, 4096>           m_freeShaderBuffer;          // off+103114904
        bgfx::Frame::FreeHandle<bgfx::FrameBufferHandle, 128>             m_freeFrameBuffer;           // off+103123098
        bgfx::Frame::FreeHandle<bgfx::UniformHandle, 512>                 m_freeUniform;               // off+103123356
        bgfx::Frame::FreeHandle<bgfx::AccelerationStructureHandle, 32768> m_freeAccelerationStructure; // off+103124382

        bgfx::TextVideoMem *m_textVideoMem; // off+103189920
        // bgfx::Stats                   m_perfStats;      // off+103189928
        // bgfx::ViewStats               m_viewStats[256]; // off+??
        char                          _fill103189928[103263856 - 103189928];
        __int64                       m_waitSubmit; // off+103263856
        __int64                       m_waitRender; // off+103263864
        bgfx::RayTracingConfiguration m_rtConfig;   // off+103263872
        bool                          m_capture;    // off+103264536
        bool                          m_skipFlip;   // off+103264537

        SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x40\x38\x35")
        SDK_API void create();

        SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\xC7\x86\x00\x00\x00\x00\x00\x00\x00\x00\x32\xC0")
        SDK_API void destroy();
    };

    // size: 7176
    class UniformRegistry {
    public:
        using UniformHashMap = bx::HandleHashMapT<1024>;

        UniformHashMap       m_uniforms;  // off+0
        bgfx::UniformRegInfo m_info[512]; // off+6152
    };

    // size: 56
    class NonLocalAllocator {
    public:
        static constexpr const uint64_t kInvalidBlock = -1;
        // size: 16
        struct Allocation {
            uint64_t m_ptr;    // off+0
            uint32_t m_offset; // off+8
        };
        // size: 16
        struct Free {
            uint64_t m_ptr;  // off+0
            uint32_t m_size; // off+8
        };
        uint64_t m_free[3]; // off+0
        uint64_t m_used[4]; // off+24
    };

    // size: 8 (1.21.50)
    struct __declspec(novtable) RendererContextI {
        // vtb+0
        virtual ~RendererContextI() = 0;

        // vtb+1
        virtual RendererType::Enum getRendererType() const = 0;

        // vtb+2
        virtual uint64_t getRendererVersion() const = 0;

        // vtb+3
        virtual uint64_t getShaderModel() const = 0;

        // vtb+4
        virtual const char *getRendererName() const = 0;

        // vtb+5
        virtual bool isDeviceRemoved() = 0;

        // vtb+6
        virtual void flip() = 0;

        // vtb+7
        virtual void postFlip() = 0;

        // vtb+8
        virtual void finishAll() = 0;

        // vtb+9
        virtual void unk_virtual_9() = 0;

        // vtb+10
        virtual void createIndexBuffer(bgfx::IndexBufferHandle _handle, const bgfx::Memory *_mem, uint16_t _flags) = 0;

        // vtb+11
        virtual void destroyIndexBuffer(bgfx::IndexBufferHandle _handle) = 0;

        // vtb+12
        virtual void createVertexDecl(bgfx::VertexDeclHandle _handle, const bgfx::VertexDecl &_decl) = 0;

        // vtb+13
        virtual void destroyVertexDecl(bgfx::VertexDeclHandle _handle) = 0;

        // vtb+14
        virtual void createVertexBuffer(
            bgfx::VertexBufferHandle _handle, const bgfx::Memory *_mem, bgfx::VertexDeclHandle _declHandle, uint16_t _flags
        ) = 0;

        // vtb+15
        virtual void destroyVertexBuffer(bgfx::VertexBufferHandle) = 0;

        // vtb+16
        virtual void createDynamicIndexBuffer(bgfx::IndexBufferHandle _handle, uint32_t _size, uint16_t _flags) = 0;

        // vtb+17
        virtual void updateDynamicIndexBuffer(bgfx::IndexBufferHandle _handle, uint32_t _offset, uint32_t _size, const bgfx::Memory *_mem) = 0;

        // vtb+18
        virtual void destroyDynamicIndexBuffer(bgfx::IndexBufferHandle _handle) = 0;

        // vtb+19
        virtual void createDynamicVertexBuffer(bgfx::VertexBufferHandle _handle, uint32_t _size, uint16_t _flags) = 0;

        // vtb+20
        virtual void updateDynamicVertexBuffer(bgfx::VertexBufferHandle _handle, uint32_t _offset, uint32_t _size, const bgfx::Memory *_mem) = 0;

        // vtb+21
        virtual void destroyDynamicVertexBuffer(bgfx::VertexBufferHandle _handle) = 0;

        // vtb+22
        virtual void createShader(bgfx::ShaderHandle _handle, const bgfx::Memory *_mem) = 0;

        // vtb+23
        virtual void destroyShader(bgfx::ShaderHandle _handle) = 0;

        // vtb+24
        virtual void createProgram(bgfx::ProgramHandle _handle, bgfx::ShaderHandle _vsh, bgfx::ShaderHandle _fsh) = 0;

        // vtb+25
        virtual void destroyProgram(bgfx::ProgramHandle _handle) = 0;

        // vtb+26
        virtual void *createTexture(bgfx::TextureHandle _handle, const bgfx::Memory *_mem, uint32_t _flags, uint8_t _skip) = 0;

        // vtb+27
        virtual void updateTextureBegin(bgfx::TextureHandle _handle, uint8_t _side, uint8_t _mip) = 0;

        // vtb+28
        virtual void updateTexture(
            bgfx::TextureHandle _handle,
            uint8_t             _side,
            uint8_t             _mip,
            const bgfx::Rect   &_rect,
            uint16_t            _z,
            uint16_t            _depth,
            uint16_t            _pitch,
            const bgfx::Memory *_mem
        ) = 0;

        // vtb+29
        virtual void updateTextureEnd() = 0;

        // vtb+30
        virtual void readTexture(bgfx::TextureHandle _handle, void *_data, uint8_t _mip) = 0;

        // vtb+31
        virtual void resizeTexture(bgfx::TextureHandle _handle, uint16_t _width, uint16_t _height, uint8_t _numMips, uint16_t _numLayers) = 0;

        // vtb+32
        virtual void overrideInternal(bgfx::TextureHandle _handle, uint64_t _ptr, uint32_t _flags) = 0;

        // vtb+33
        virtual uint64_t getInternal(bgfx::TextureHandle _handle) = 0;

        // vtb+34
        virtual void destroyTexture(bgfx::TextureHandle _handle) = 0;

        // vtb+35
        virtual void wrapExternalTexture(
            bgfx::TextureHandle _handle, bgfx::RendererType::Enum _type, void *_texturePtr, void *_deferredRef
        ) {}

        // vtb+36
        virtual void createShaderBuffer(bgfx::ShaderBufferHandle _handle, uint32_t _size, uint32_t _stride) = 0;

        // vtb+37
        virtual void updateShaderBuffer(bgfx::ShaderBufferHandle _handle, const bgfx::Memory *_mem) = 0;

        // vtb+38
        virtual void destroyShaderBuffer(bgfx::ShaderBufferHandle _handle) {}

        // vtb+39
        virtual void createFrameBuffer(
            bgfx::FrameBufferHandle _handle, void *_nwh, uint32_t _width, uint32_t _height, bgfx::TextureFormat::Enum _depthFormat
        ) = 0;

        // vtb+40
        virtual void createFrameBuffer(bgfx::FrameBufferHandle _handle, uint8_t _num, const bgfx::Attachment *_attachment) = 0;

        // vtb+41
        virtual void destroyFrameBuffer(bgfx::FrameBufferHandle _handle) = 0;

        // vtb+42
        virtual void createUniform(bgfx::UniformHandle _handle, bgfx::UniformType::Enum _type, uint16_t _num, const char *_name) = 0;

        // vtb+43
        virtual void destroyUniform(bgfx::UniformHandle _handle) = 0;

        // vtb+44
        virtual void requestScreenShot(bgfx::FrameBufferHandle _handle, const char *_filePath) = 0;

        // vtb+45
        virtual void updateViewName(uint16_t _id, const char *_name) = 0;

        // vtb+46
        virtual void updateUniform(void *_uniformsTarget, uint16_t _loc, const void *_data, uint32_t _size) = 0;

        // vtb+47
        virtual void setMarker(void *_marker, const char *, uint32_t _size) = 0;

        // vtb+48
        virtual void invalidateOcclusionQuery(bgfx::OcclusionQueryHandle _handle) = 0;

        // vtb+49
        virtual void setName(bgfx::Handle _handle, const char *_name) = 0;

        // vtb+50
        virtual void submit(bgfx::Frame *_render, bgfx::ClearQuad &_clearQuad, bgfx::TextVideoMemBlitter &_textVideoMemBlitter) = 0;

        // vtb+51
        virtual void blitSetup(bgfx::TextVideoMemBlitter &_blitter) = 0;

        // vtb+52
        virtual void blitRender(bgfx::TextVideoMemBlitter &_blitter, uint32_t _numIndices) = 0;

        // vtb+53
        virtual void unk_virtual_53() = 0;

        // vtb+54
        virtual void createBottomLevelAccelerationStructure(
            bgfx::AccelerationStructureHandle           _handle,
            bgfx::VertexBufferHandle                    _vertexBufferHandle,
            bgfx::VertexDeclHandle                      _vertexDeclHandle,
            uint32_t                                    _vertexCount,
            uint32_t                                    _vertexOffset,
            bgfx::IndexBufferHandle                     _indexBufferHandle,
            uint32_t                                    _indexCount,
            uint32_t                                    _indexOffset,
            uint16_t                                    indexSize,
            bgfx::AccelerationStructureBuildFlags::Enum _flags
        ) {}

        // vtb+55
        virtual void createTopLevelAccelerationStructure(
            bgfx::AccelerationStructureHandle _handle, uint32_t numInstances, bgfx::TopLevelInstanceDesc *instanceDescs
        ) {}

        // vtb+56
        virtual void destroyAccelerationStructureBuffer(
            bgfx::AccelerationStructureHandle _handle
        ) {}
    };

    inline RendererContextI::~RendererContextI() = default;

    // 1.21.50
    struct Context {
        bx::Semaphore m_renderSem;       // off+0
        bx::Semaphore m_apiSem;          // off+128
        bx::Semaphore m_encoderEndSem;   // off+256
        bx::Mutex     m_encoderApiLock;  // off+384
        bx::Mutex     m_resourceApiLock; // off+448

        char _fill512[206604232 - 448 - sizeof(m_resourceApiLock)]; // off+512

        Frame                          *m_submit;                              // off+206604232
        uint64_t                        m_tempKeys[65535];                     // off+206604240
        uint16_t                        m_tempValues[65535];                   // off+207128520
        bgfx::IndexBuffer               m_indexBuffers[4096];                  // off+207259592
        bgfx::VertexBuffer              m_vertexBuffers[4096];                 // off+207275976
        bgfx::DynamicIndexBuffer        m_dynamicIndexBuffers[4096];           // off+207308744
        bgfx::DynamicVertexBuffer       m_dynamicVertexBuffers[4096];          // off+207390664
        uint16_t                        m_numFreeDynamicIndexBufferHandles;    // off+207505352
        uint16_t                        m_numFreeDynamicVertexBufferHandles;   // off+207505354
        uint16_t                        m_numFreeOcclusionQueryHandles;        // off+207505356
        bgfx::DynamicIndexBufferHandle  m_freeDynamicIndexBufferHandle[4096];  // off+207505358
        bgfx::DynamicVertexBufferHandle m_freeDynamicVertexBufferHandle[4096]; // off+207513550
        bgfx::OcclusionQueryHandle      m_freeOcclusionQueryHandle[256];       // off+207521742
        bgfx::NonLocalAllocator         m_dynIndexBufferAllocator;             // off+207522256
        bx::HandleAllocT<4096>          m_dynamicIndexBufferHandle;            // off+207522312
        bgfx::NonLocalAllocator         m_dynVertexBufferAllocator;            // off+207538704
        bx::HandleAllocT<4096>          m_dynamicVertexBufferHandle;           // off+207538760
        bx::HandleAllocT<4096>          m_indexBufferHandle;                   // off+207555148
        bx::HandleAllocT<64>            m_vertexDeclHandle;                    // off+207571536
        bx::HandleAllocT<4096>          m_vertexBufferHandle;                  // off+207571796

        Context() = delete;

        SPHR_DECL_API("1.21.50", "lea", "\x4C\x8D\x05\x00\x00\x00\x00\x48\x89\x7B")
        SDK_API static int32_t renderThread(bx::Thread *, void *);

        SPHR_DECL_API("1.21.50", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x0F\xB6\xFA\xBE")
        SDK_API int init(bool a2);

        SPHR_DECL_API("1.21.50", "\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x81\xA1")
        SDK_API void initRenderer();

        SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x83\xF8\x00\x75\x00\x48\x89\x7C\x24")
        SDK_API bgfx::RenderFrame::Enum renderFrame(int32_t _msecs);

        SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x80\xBF\x00\x00\x00\x00\x00\x48\xC7\x05")
        SDK_API bool apiSemWait(int32_t _msecs);

        SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x41\xFF\xC1\x41\x81\xF9")
        SDK_API void resetView(uint16_t _id);

        SPHR_DECL_API("1.21.50", "\x40\x53\x48\x83\xEC\x00\x4C\x8B\x81\x00\x00\x00\x00\x48\x8B\xD9\x49\x81\xC0")
        SDK_API void frameNoRenderWait(bool a2);

        SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x41\x80\xBE\x00\x00\x00\x00\x00\x75\x00\x45\x33\xC0")
        SDK_API void swap();

        SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\xBF")
        SDK_API void freeDynamicBuffers();

        SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x4C\x8B\x93\x00\x00\x00\x00\x4C\x8B\xCE")
        SDK_API void freeAllHandles(bgfx::Frame *_frame);

        SPHR_DECL_API("1.21.50", "\x40\x53\x55\x56\x57\x41\x56\x48\x83\xEC\x00\x45\x0F\xB7\xF1")
        SDK_API bgfx::IndexBufferHandle createIndexBuffer(const bgfx::Memory *_mem, uint16_t _flags = BGFX_BUFFER_NONE);

        SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x48\x8B\xCB\xE8\x00\x00\x00\x00\x48\x85\xC0\x75\x00\x0F\xB7\x95\x00\x00\x00\x00\xBF\x00\x00\x00\x00\x48\x83\xC2\x00\x49\x83\xC4\x00\x48\x8D\x04\x32")
        SDK_API void destroyIndexBuffer(bgfx::IndexBufferHandle _handle);

        SPHR_DECL_API("1.21.50", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x4D\x8B\xE1\x49\x8B\xF8")
        SDK_API bgfx::VertexBufferHandle createVertexBuffer(
            const bgfx::Memory *_mem, const bgfx::VertexDecl &_decl, uint16_t _flags = BGFX_BUFFER_NONE
        );

        SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x0F\xB7\x08\x66\x89\x0B\xC6\x43\x00\x00\xEB\x00\x80\xBA")
        SDK_API bgfx::DynamicVertexBufferHandle createDynamicVertexBuffer(
            uint32_t _num, const bgfx::VertexDecl &_decl, uint16_t _flags = BGFX_BUFFER_NONE
        );

        SPHR_DECL_API("1.21.50", "\x40\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x45\x0F\xB7\xF9")
        SDK_API bgfx::DynamicIndexBufferHandle createDynamicIndexBuffer(
            uint32_t _num, uint16_t _flags = BGFX_BUFFER_NONE
        );

        SPHR_DECL_API("1.21.50", "\x40\x53\x55\x56\x57\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x00\x4D\x8B\xF9\x45\x8B\xE0")
        SDK_API void update(bgfx::DynamicVertexBufferHandle _handle, uint32_t _startVertex, const bgfx::Memory *_mem);

        SPHR_DECL_API("1.21.50", "\x40\x53\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x4D\x8B\xE9\x45\x8B\xE0")
        SDK_API void update(bgfx::DynamicIndexBufferHandle _handle, uint32_t _startIndex, const bgfx::Memory *_mem);

        SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x87\x83\x00\x00\x00\x00\x48\x8B\x4B\x00\xC6\x43\x00\x00\xE8")
        SDK_API uint32_t frame(uint32_t _flags);

        SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x0F\xB7\x38\xB8")
        SDK_API bgfx::ShaderHandle createShader(const bgfx::Memory *_mem);

        SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x0F\xB7\x00\x66\x3B\xD8")
        SDK_API bgfx::ProgramHandle createProgram(bgfx::ShaderHandle _vsh, bool _destroyShader);

        CommandBuffer &getCommandBuffer(CommandBuffer::Enum _cmd) {
            CommandBuffer &cmdbuf = _cmd < CommandBuffer::Enum::End
                                      ? sapphire::dAccess<CommandBuffer>(m_submit, 102553560 /*1.21.50*/)
                                      : sapphire::dAccess<CommandBuffer>(m_submit, 1145141919810 /*FIXME*/);
            uint8_t        cmd = (uint8_t)_cmd;
            cmdbuf.write(cmd);
            return cmdbuf;
        }

        IndirectBufferHandle createIndirectBuffer(uint32_t _num) {
            IndirectBufferHandle handle = {m_vertexBufferHandle.alloc()};

            if (isValid(handle)) {
                uint32_t size = _num * BGFX_CONFIG_DRAW_INDIRECT_STRIDE;
                uint16_t flags = BGFX_BUFFER_DRAW_INDIRECT;

                CommandBuffer &cmdbuf = getCommandBuffer(CommandBuffer::Enum::CreateDynamicVertexBuffer);
                cmdbuf.write(handle);
                cmdbuf.write(size);
                cmdbuf.write(flags);
            }
            return handle;
        }

        void destroyIndirectBuffer(bgfx::IndirectBufferHandle _handle); /*{
            VertexBufferHandle handle = {_handle.idx};
            CommandBuffer     &cmdbuf = sapphire::dAccess<CommandBuffer>(m_submit, 102553560);
            cmdbuf.write((uint8_t)CommandBuffer::Enum::CreateDynamicVertexBuffer);
            cmdbuf.write(handle);
            // FIXME
            // m_submit->free(handle);
        }*/
    };

    // 1.21.50
    struct EncoderImpl {
        bgfx::Frame        *m_frame;   // off+0
        bgfx::SortKey       m_key;     // off+8
        bgfx::RenderDraw    m_draw;    // off+64
        bgfx::RenderCompute m_compute; // off+192
        bgfx::RenderBind    m_bind;    // off+256

        char _fill[1612 - 256 - sizeof(bgfx::RenderBind)];

        uint32_t m_numSubmitted;   // off+1612
        uint32_t m_numDropped;     // off+1616
        uint32_t m_uniformBegin;   // off+1620
        uint32_t m_uniformEnd;     // off+1624
        uint32_t m_numVertices[4]; // off+1628
        uint8_t  m_uniformIdx;     // off+1644
        bool     m_discard;        // off+1645

        EncoderImpl() = delete;

        SPHR_DECL_API("1.21.50", "\x40\x53\x55\x56\x57\x41\x56\x41\x57\x48\x83\xEC\x00\x48\x8B\x01\x4D\x8B\xF9")
        SDK_API void setUniform(
            bgfx::UniformType::Enum _type, bgfx::UniformHandle _handle, const void *_value, uint16_t _num
        );

        SPHR_DECL_API("1.21.50", "\x48\x89\x6C\x24\x00\x48\x89\x7C\x24\x00\x41\x56\x48\x83\xEC\x00\x48\x8B\xF9")
        SDK_API void discard();

        SPHR_DECL_API("1.21.50", "\x40\x53\x48\x83\xEC\x00\x80\xB9\x00\x00\x00\x00\x00\x48\x8B\xD9\x44\x0F\xB7\xD2")
        SDK_API void submit(
            ViewId                     _id,
            bgfx::ProgramHandle        _program,
            bgfx::OcclusionQueryHandle _occlusionQuery,
            int32_t                    _depth = 0,
            bool                       _preserveState = false
        );

        void submit(
            ViewId               _id,
            ProgramHandle        _program,
            IndirectBufferHandle _indirectHandle,
            uint16_t             _start,
            uint16_t             _num,
            uint32_t             _depth = 0,
            bool                 _preserveState = false
        ) {
            m_draw.m_startIndirect = _start;
            m_draw.m_numIndirect = _num;
            m_draw.m_indirectBuffer = _indirectHandle;
            m_draw.m_indexSize = 2; // TODO: get size of an index
            OcclusionQueryHandle handle{kInvalidHandle};
            submit(_id, _program, handle, _depth, _preserveState);
        }

        void setVertexBuffer(
            uint8_t            _stream,
            VertexBufferHandle _handle,
            uint32_t           _startVertex = 0,
            uint32_t           _numVertices = UINT32_MAX,
            VertexDeclHandle   _decl = {bgfx::kInvalidHandle}
        ) {
            if (m_draw.setStreamBit(_stream, _handle)) {
                Stream &stream = m_draw.m_stream[_stream];
                stream.m_startVertex = _startVertex;
                stream.m_handle = _handle;
                stream.m_decl = _decl;
                m_numVertices[_stream] = _numVertices;
            }
        }

        void setIndexBuffer(IndexBufferHandle _handle, uint32_t _firstIndex = 0, uint32_t _numIndices = UINT32_MAX) {
            m_draw.m_startIndex = _firstIndex;
            m_draw.m_numIndices = _numIndices;
            m_draw.m_indexBuffer = _handle;
        }

        void setInstanceDataBuffer(const InstanceDataBuffer *_idb, uint32_t _start = 0, uint32_t _num = UINT32_MAX) {
            const uint32_t start = std::min(_start, _idb->num);
            const uint32_t num = std::min(_idb->num - start, _num);
            m_draw.m_instanceDataOffset = _idb->offset + start * _idb->stride;
            m_draw.m_instanceDataStride = _idb->stride;
            m_draw.m_numInstances = num;
            m_draw.m_instanceDataBuffer = _idb->handle;
        }
    };

    SPHR_DECL_API("1.21.50", "\x48\x89\x54\x24\x00\x4C\x89\x44\x24\x00\x4C\x89\x4C\x24\x00\x55\x53\x56\x57\x41\x54\x41\x55")
    SDK_API void fatal(bgfx::Fatal::Enum _code, const char *_format, ...);

    template <typename... Args>
    void fatal(bgfx::Fatal::Enum _code, const char *_format, Args &&...args) {
        fpFatal(_code, _format, std::forward<Args>(args)...);
    }

} // namespace bgfx