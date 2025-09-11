#pragma once

#include "bgfx_p.h"
#include "dxgi.h"
#include "NvApi.h"
#include "ringbuffer.h"
#include "renderer_d3d12.h"
#include "SDK/api/src-external/bx/ringbuffer.h"

namespace bgfx {
    struct RayTracingConfiguration;
    struct RenderBind;
} // namespace bgfx

namespace bgfx::d3d12 {

    struct CommandList;
    struct CommandListD3D12;

    // size: 24
    struct HeapProperty {
        enum class Enum : int32_t {
            Default = 0,
            Texture = 1,
            Upload = 2,
            ReadBack = 3,
            Display = 4,
            AccelerationStructure = 5,
            Custom = 6,
            Count = 7,
        };

        D3D12_HEAP_PROPERTIES m_properties; // off+0
        D3D12_RESOURCE_STATES m_state;      // off+20
    };

    // size: 32
    class StagingBufferD3D12 {
    public:
        ID3D12Resource                 *m_upload;     // off+0
        uint64_t                        m_gpuVA;      // off+8
        uint8_t                        *m_data;       // off+16
        uint32_t                        m_size;       // off+24
        bgfx::d3d12::HeapProperty::Enum m_properties; // off+28
    };

    template <size_t I>
    struct RingBufferAllocator {};

    // size: 56
    struct FenceSyncContext {
        using BufferTuple = std::tuple<void *, std::function<void(size_t)>, std::function<void(size_t)>>;

        std::vector<BufferTuple> m_registeredBuffers; // off+0
        ID3D12Fence             *m_fence;             // off+24
        void                    *m_event;             // off+32
        uint64_t                 m_currentNumber;     // off+40
        uint64_t                 m_completedNumber;   // off+48
    };

    struct CommandContextD3D12 {};

    // size: 63265872 (1.21.50)
    struct RendererContextD3D12 : public bgfx::RendererContextI {
        ID3D12DescriptorHeap *m_rtvDescriptorHeap;       // off+8
        ID3D12DescriptorHeap *m_dsvDescriptorHeap;       // off+16
        ID3D12Resource       *m_backBufferColor[4];      // off+24
        uint64_t              m_backBufferColorFence[4]; // off+56
        ID3D12Resource       *m_backBufferDepthStencil;  // off+88
        std::vector<void *>   m_externalTextureCache;    // off+96
        uint32_t              m_externalSwapchainIdx;    // off+120
        uint32_t              mUnk124;                   // off+124
        bool                  m_isDebug;                 // off+128
        bool                  m_useBufferHeaps;          // off+129
        bgfx::Dxgi            m_dxgi;                    // off+136
        bgfx::NvApi           m_nvapi;                   // off+584
        void                 *m_kernel32dll;             // off+632
        void                 *m_d3d12dll;                // off+640
        void                 *m_renderdocdll;            // off+648
        void                 *m_winPixEvent;             // off+656

        std::unique_ptr<bgfx::RingBuffer<StagingBufferD3D12, size_t, void *, RingBufferAllocator<2>>> m_uploadBuffer; // off+664
        static_assert(sizeof(bgfx::RingBuffer<StagingBufferD3D12, size_t, void *, RingBufferAllocator<2>>) == 112);

        bgfx::d3d12::FenceSyncContext    m_sync;                       // off+672
        D3D_FEATURE_LEVEL                m_featureLevel;               // off+728
        D3D12_FEATURE_DATA_SHADER_MODEL  m_shaderModel;                // off+732
        D3D_DRIVER_TYPE                  m_driverType;                 // off+736
        D3D12_FEATURE_DATA_ARCHITECTURE  m_architecture;               // off+740
        D3D12_FEATURE_DATA_D3D12_OPTIONS m_options;                    // off+756
        HANDLE                           m_FrameLatencyWaitableObject; // off+816
        IDXGISwapChain3                 *m_swapChain;                  // off+824
        ID3D12Resource                  *m_msaaRenderTarget;           // off+832
        int64_t                          m_presentElapsed;             // off+840
        uint16_t                         m_numWindows;                 // off+848
        bgfx::FrameBufferHandle          m_windows[128];               // off+850
        ID3D12Device                    *m_device;                     // off+1112

        char _fill1120[32566 - 1112 - 8];

        // bgfx::d3d12::TimerQueryD3D12     m_gpuTimer;       // off+

        // bgfx::d3d12::OcclusionQueryD3D12 m_occlusionQuery; // off+32008

        char m_viewName[256][256]; // off+32566

        char _fill98102[104288 - 32566 - 256 * 256];

        bgfx::d3d12::BufferHeap *m_bufferHeaps[7]; // off+104288

        ID3D12CommandSignature   *m_commandSignature[3]; // off+104344
        bgfx::d3d12::CommandList *m_globalCommands;      // off+104368

        std::vector<std::unique_ptr<CommandContextD3D12>> m_commandContext; // off+104376
        bgfx::Resolution                                  m_resolution;     // off+104400
        bool                                              m_wireframe;      // off+104412
        bool                                              m_fatal;          // off+104413
        bgfx::FatalError                                  m_fatalError;     // off+114416
        bgfx::SwapChainDesc                               m_scd;            // off+104424
        uint32_t                                          m_maxAnisotropy;  // off+104496
        bool                                              m_depthClamp;     // off+104500
        bool                                              mUnk104501;       // off+104501

        char _fill104502[9673168 - 104501 - 1];

        bgfx::d3d12::BufferD3D12       m_indexBuffers[4096];  // off+9673168
        bgfx::d3d12::VertexBufferD3D12 m_vertexBuffers[4096]; // off+10164688
        bgfx::d3d12::BufferD3D12       m_shaderBuffers[4096]; // off+10688976
        bgfx::d3d12::ShaderD3D12       m_shaders[4096];       // off+11180496
        bgfx::d3d12::ProgramD3D12      m_program[2048];       // off+11934160
        bgfx::d3d12::TextureD3D12      m_textures[4096];      // off+12442064

        char _fill13195728[32];

        bgfx::VertexDecl              m_vertexDecls[64];   // off+13195760
        bgfx::d3d12::FrameBufferD3D12 m_frameBuffers[128]; // off+13201136
        bgfx::UniformRegistry         m_uniformReg;        // off+13214448

        bgfx::TextVideoMem m_textVideoMem;       // off+13221624
        uint32_t           m_backBufferColorIdx; // off+13221648

        char _fill13221652[63265792 - 13221648 - 4];

        // bool                                m_directAccessSupport; // off+
        // bgfx::d3d12::ResourceStatesMananger m_states;              // off+

        // bgfx::d3d12::CommandQueueD3D12 m_cmd; // off+13404840

        std::mutex m_cmdCreateMtx; // off+63265792

        // vtb+57
        virtual bool init(const bgfx::Init &_init);

        // vtb+58
        virtual void shutdown();

        // vtb+59
        virtual bool updateResolution(const bgfx::Resolution &_resolution);

        // vtb+60
        virtual void kick(bool _alloc);

        // vtb+61
        virtual void finish();

        // vtb+62
        virtual void unk_virtual_62();

        // vtb+63
        virtual void rayTrace(
            bgfx::d3d12::CommandListD3D12       &bgfxCommandList,
            const bgfx::RayTracingConfiguration &rtConfig,
            const bgfx::RenderBind              &renderBind,
            const bgfx::Matrix4                 &viewMat,
            const bgfx::Matrix4                 &projMat
        );
    };
} // namespace bgfx::d3d12
