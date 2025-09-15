#pragma once

#include <d3d12.h>
#include "bgfx.h"
#include "SDK/api/src-external/bx/handlealloc.h"
#include "SDK/api/src-external/bx/float4x4_t.h"
#include "SDK/api/src-external/bx/thread.h"
#include "SDK/api/src-external/bx/semaphore.h"
#include "SDK/api/src-external/bx/mutex.h"
#include "platform.h"

#define BGFX_SUBMIT_INTERNAL_NONE UINT8_C(0x00)
#define BGFX_SUBMIT_INTERNAL_INDEX32 UINT8_C(0x40)
#define BGFX_SUBMIT_INTERNAL_OCCLUSION_VISIBLE UINT8_C(0x80)
#define BGFX_SUBMIT_INTERNAL_RESERVED_MASK UINT8_C(0xff)

namespace bgfx {

    struct Frame;

    struct ClearQuad;

    struct TextVideoMemBlitter;

    struct TopLevelInstanceDesc;

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
        uint8_t  m_buffer[114514]; // off+8, todo: size
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

    // size: 1280
    struct alignas(64) RenderBind {
        bgfx::Binding m_bind[64]; // off+0
    };

    struct Frame {
        SDK_API void create();

        SDK_API void destroy();
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
        virtual void createIndexBuffer(bgfx::IndexBufferHandle, const bgfx::Memory *, uint16_t) = 0;

        // vtb+11
        virtual void destroyIndexBuffer(bgfx::IndexBufferHandle) = 0;

        // vtb+12
        virtual void createVertexDecl(bgfx::VertexDeclHandle, const bgfx::VertexDecl &) = 0;

        // vtb+13
        virtual void destroyVertexDecl(bgfx::VertexDeclHandle) = 0;

        // vtb+14
        virtual void createVertexBuffer(
            bgfx::VertexBufferHandle, const bgfx::Memory *, bgfx::VertexDeclHandle, uint16_t
        ) = 0;

        // vtb+15
        virtual void destroyVertexBuffer(bgfx::VertexBufferHandle) = 0;

        // vtb+16
        virtual void createDynamicIndexBuffer(bgfx::IndexBufferHandle, uint32_t, uint16_t) = 0;

        // vtb+17
        virtual void updateDynamicIndexBuffer(bgfx::IndexBufferHandle, uint32_t, uint32_t, const bgfx::Memory *) = 0;

        // vtb+18
        virtual void destroyDynamicIndexBuffer(bgfx::IndexBufferHandle) = 0;

        // vtb+19
        virtual void createDynamicVertexBuffer(bgfx::VertexBufferHandle, uint32_t, uint16_t) = 0;

        // vtb+20
        virtual void updateDynamicVertexBuffer(bgfx::VertexBufferHandle, uint32_t, uint32_t, const bgfx::Memory *) = 0;

        // vtb+21
        virtual void destroyDynamicVertexBuffer(bgfx::VertexBufferHandle) = 0;

        // vtb+22
        virtual void createShader(bgfx::ShaderHandle, const bgfx::Memory *) = 0;

        // vtb+23
        virtual void destroyShader(bgfx::ShaderHandle) = 0;

        // vtb+24
        virtual void createProgram(bgfx::ProgramHandle, bgfx::ShaderHandle, bgfx::ShaderHandle) = 0;

        // vtb+25
        virtual void destroyProgram(bgfx::ProgramHandle) = 0;

        // vtb+26
        virtual void *createTexture(bgfx::TextureHandle, const bgfx::Memory *, uint32_t, uint8_t) = 0;

        // vtb+27
        virtual void updateTextureBegin(bgfx::TextureHandle, uint8_t, uint8_t) = 0;

        // vtb+28
        virtual void updateTexture(
            bgfx::TextureHandle,
            uint8_t,
            uint8_t,
            const bgfx::Rect &,
            uint16_t,
            uint16_t,
            uint16_t,
            const bgfx::Memory *
        ) = 0;

        // vtb+29
        virtual void updateTextureEnd() = 0;

        // vtb+30
        virtual void readTexture(bgfx::TextureHandle, void *, uint8_t) = 0;

        // vtb+31
        virtual void resizeTexture(bgfx::TextureHandle, uint16_t, uint16_t, uint8_t, uint16_t) = 0;

        // vtb+32
        virtual void overrideInternal(bgfx::TextureHandle, uint64_t, uint32_t) = 0;

        // vtb+33
        virtual uint64_t getInternal(bgfx::TextureHandle) = 0;

        // vtb+34
        virtual void destroyTexture(bgfx::TextureHandle) = 0;

        // vtb+35
        virtual void wrapExternalTexture(
            bgfx::TextureHandle _handle, bgfx::RendererType::Enum _type, void *_texturePtr, void *ref
        ) {}

        // vtb+36
        virtual void createShaderBuffer(bgfx::ShaderBufferHandle, uint32_t, uint32_t) = 0;

        // vtb+37
        virtual void updateShaderBuffer(bgfx::ShaderBufferHandle, const bgfx::Memory *) = 0;

        // vtb+38
        virtual void destroyShaderBuffer(bgfx::ShaderBufferHandle _handle) {}

        // vtb+39
        virtual void createFrameBuffer(
            bgfx::FrameBufferHandle, void *, uint32_t, uint32_t, bgfx::TextureFormat::Enum
        ) = 0;

        // vtb+40
        virtual void createFrameBuffer(bgfx::FrameBufferHandle, uint8_t, const bgfx::Attachment *) = 0;

        // vtb+41
        virtual void destroyFrameBuffer(bgfx::FrameBufferHandle) = 0;

        // vtb+42
        virtual void createUniform(bgfx::UniformHandle, bgfx::UniformType::Enum, uint16_t, const char *) = 0;

        // vtb+43
        virtual void destroyUniform(bgfx::UniformHandle) = 0;

        // vtb+44
        virtual void requestScreenShot(bgfx::FrameBufferHandle, const char *) = 0;

        // vtb+45
        virtual void updateViewName(uint16_t, const char *) = 0;

        // vtb+46
        virtual void updateUniform(void *, uint16_t, const void *, uint32_t) = 0;

        // vtb+47
        virtual void setMarker(void *, const char *, uint32_t) = 0;

        // vtb+48
        virtual void invalidateOcclusionQuery(bgfx::OcclusionQueryHandle) = 0;

        // vtb+49
        virtual void setName(bgfx::Handle, const char *) = 0;

        // vtb+50
        virtual void submit(bgfx::Frame *, bgfx::ClearQuad &, bgfx::TextVideoMemBlitter &) = 0;

        // vtb+51
        virtual void blitSetup(bgfx::TextVideoMemBlitter &) = 0;

        // vtb+52
        virtual void blitRender(bgfx::TextVideoMemBlitter &, uint32_t) = 0;

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

        SDK_API static int32_t renderThread(bx::Thread *, void *);

        SDK_API int init(bool a2);

        SDK_API void initRenderer();

        SDK_API bgfx::RenderFrame::Enum renderFrame(int32_t _msecs);

        SDK_API bool apiSemWait(int32_t _msecs);

        SDK_API void resetView(uint16_t _id);

        SDK_API void frameNoRenderWait(bool a2);

        SDK_API void swap();

        SDK_API void freeDynamicBuffers();

        SDK_API void freeAllHandles(bgfx::Frame *_frame);

        SDK_API bgfx::IndexBufferHandle createIndexBuffer(const bgfx::Memory *_mem, uint16_t _flags = BGFX_BUFFER_NONE);

        SDK_API void destroyIndexBuffer(bgfx::IndexBufferHandle _handle);

        SDK_API bgfx::VertexBufferHandle createVertexBuffer(
            const bgfx::Memory *_mem, const bgfx::VertexDecl &_decl, uint16_t _flags = BGFX_BUFFER_NONE
        );

        SDK_API bgfx::DynamicVertexBufferHandle createDynamicVertexBuffer(
            uint32_t _num, const bgfx::VertexDecl &_decl, uint16_t _flags = BGFX_BUFFER_NONE
        );

        SDK_API bgfx::DynamicIndexBufferHandle createDynamicIndexBuffer(
            uint32_t _num, uint16_t _flags = BGFX_BUFFER_NONE
        );

        SDK_API void update(bgfx::DynamicVertexBufferHandle _handle, uint32_t _startVertex, const bgfx::Memory *_mem);

        SDK_API void update(bgfx::DynamicIndexBufferHandle _handle, uint32_t _startIndex, const bgfx::Memory *_mem);

        SDK_API uint32_t frame(uint32_t _flags);

        SDK_API bgfx::ShaderHandle createShader(const bgfx::Memory *_mem);

        SDK_API bgfx::ProgramHandle createProgram(bgfx::ShaderHandle _vsh, bool _destroyShader);

        CommandBuffer &getCommandBuffer(CommandBuffer::Enum _cmd) {
            CommandBuffer &cmdbuf = _cmd < CommandBuffer::Enum::End
                                      ? memory::getField<CommandBuffer>(m_submit, 102553560 /*1.21.50*/)
                                      : memory::getField<CommandBuffer>(m_submit, 1145141919810 /*FIXME*/);
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
            CommandBuffer     &cmdbuf = memory::getField<CommandBuffer>(m_submit, 102553560);
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

        SDK_API void setUniform(
            bgfx::UniformType::Enum _type, bgfx::UniformHandle _handle, const void *_value, uint16_t _num
        );

        SDK_API void discard();

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

    SDK_API void fatal(bgfx::Fatal::Enum _code, const char *_format, ...);
    namespace sphr_none_export {
        extern SDK_API decltype(&fatal) fpFatal;
    }

    template <typename... Args>
    void fatal(bgfx::Fatal::Enum _code, const char *_format, Args &&...args) {
        fpFatal(_code, _format, std::forward<Args>(args)...);
    }

} // namespace bgfx