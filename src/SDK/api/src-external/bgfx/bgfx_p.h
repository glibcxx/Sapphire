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
    }; // struct bgfx::Matrix4

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

    // size: 7176
    class UniformRegistry {
    public:
        using UniformHashMap = bx::HandleHashMapT<1024, uint32_t>;

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

} // namespace bgfx