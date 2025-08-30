#pragma once

#include <d3d12.h>
#include "bgfx.h"
#include "SDK/api/src-external/bx/handlealloc.h"
#include "SDK/api/src-external/bx/float4x4_t.h"

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
            // ...
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

    // size: 8
    struct Stream {
        uint32_t                 m_startVertex; // off+0
        bgfx::VertexBufferHandle m_handle;      // off+4
        bgfx::VertexDeclHandle   m_decl;        // off+6
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
        uint32_t                   mUnk60 = 0;               // off+72
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
    };

    // size: 7176
    class UniformRegistry {
    public:
        using UniformHashMap = bx::HandleHashMapT<1024>;

        UniformHashMap       m_uniforms;  // off+0
        bgfx::UniformRegInfo m_info[512]; // off+6152
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
        char _fill0[206604232];

        Frame *m_submit; // off+206604232

        char _fill206604240[950924];

        bx::HandleAllocT<4096> m_indexBufferHandle;  // off+207555148
        bx::HandleAllocT<64>   m_vertexDeclHandle;   // off+207571536
        bx::HandleAllocT<4096> m_vertexBufferHandle; // off+207571796

        Context() = delete;

        SDK_API bgfx::IndexBufferHandle createIndexBuffer(const bgfx::Memory *_mem, uint16_t _flags = 0x0000);

        SDK_API void destroyIndexBuffer(bgfx::IndexBufferHandle _handle);

        SDK_API bgfx::VertexBufferHandle createVertexBuffer(
            const bgfx::Memory *_mem, const bgfx::VertexDecl &_decl, uint16_t _flags = 0x0000
        );

        SDK_API bgfx::DynamicVertexBufferHandle createDynamicVertexBuffer(
            uint32_t _num, const bgfx::VertexDecl &_decl, uint16_t _flags = 0x0000
        );

        SDK_API bgfx::DynamicIndexBufferHandle createDynamicIndexBuffer(uint32_t _num, uint16_t _flags = 0x0000);

        SDK_API uint32_t frame(uint32_t _flags);

        IndirectBufferHandle createIndirectBuffer(uint32_t _num) {
            IndirectBufferHandle handle = {m_vertexBufferHandle.alloc()};

            if (isValid(handle)) {
                uint32_t size = _num * 32;
                uint16_t flags = 0x0400;

                CommandBuffer &cmdbuf = memory::getField<CommandBuffer>(m_submit, 102553560 /*1.21.50*/);
                cmdbuf.write((uint8_t)CommandBuffer::Enum::CreateDynamicVertexBuffer);
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
        bgfx::Frame     *m_frame; // off+0
        bgfx::SortKey    m_key;   // off+8
        bgfx::RenderDraw m_draw;  // off+64

        char _fill[1612 - 64 - sizeof(bgfx::RenderDraw)];

        uint32_t m_numSubmitted;   // off+1612
        uint32_t m_numDropped;     // off+1616
        uint32_t m_uniformBegin;   // off+1620
        uint32_t m_uniformEnd;     // off+1624
        uint32_t m_numVertices[4]; // off+1628
        uint8_t  m_uniformIdx;     // off+1644
        bool     m_discard;        // off+1645

        EncoderImpl() = delete;

        void setVertexBuffer(uint8_t, bgfx::VertexBufferHandle, uint32_t, uint32_t);

        SDK_API void setUniform(
            bgfx::UniformType::Enum _type, bgfx::UniformHandle _handle, const void *_value, uint16_t _num
        );

        SDK_API void discard();

        SDK_API void submit(
            ViewId                     _id,
            bgfx::ProgramHandle        _program,
            bgfx::OcclusionQueryHandle _occlusionQuery,
            int32_t                    _depth,
            bool                       _preserveState
        );

        void submit(
            ViewId               _id,
            ProgramHandle        _program,
            IndirectBufferHandle _indirectHandle,
            uint16_t             _start,
            uint16_t             _num,
            uint32_t             _depth,
            bool                 _preserveState = false
        ) {
            m_draw.m_startIndirect = _start;
            m_draw.m_numIndirect = _num;
            m_draw.m_indirectBuffer = _indirectHandle;
            OcclusionQueryHandle handle{kInvalidHandle};
            submit(_id, _program, handle, _depth, _preserveState);
        }
    };

} // namespace bgfx