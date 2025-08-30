#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <mutex>
#include "bgfx.h"
#include "bgfx_p.h"

namespace bgfx {

    struct Memory;
    class UniformBuffer;

    // size: 8
    struct FatalError {
        bgfx::Fatal::Enum m_error;   // off+0
        HRESULT           m_hResult; // off+4
    };

    namespace d3d12 {

        // size: 8
        struct ResourceD3D12 {
            ID3D12Resource *m_ptr; // off+0
        };

        // size: 120 (1.21.50)
        struct BufferD3D12 : public ResourceD3D12 {
            enum class Usage : int {
                USAGE_VERTEX_BUFFER = 0,
                USAGE_INDEX_BUFFER = 1,
                USAGE_SHADER_BUFFER = 2,
                USAGE_INDIRECT_BUFFER = 3,
                USAGE_RAYTRACING_ACCELERATION_STRUCTURE = 4,
            };

            D3D12_SHADER_RESOURCE_VIEW_DESC  m_srvd;             // off+8
            D3D12_UNORDERED_ACCESS_VIEW_DESC m_uavd;             // off+48
            uint64_t                         m_gpuVA;            // off+88
            uint32_t                         m_size;             // off+96
            uint16_t                         m_flags;            // off+100
            bool                             m_dynamic;          // off+102
            void                            *m_mappedCpuAddress; // off+104
            uint32_t                         m_stride;           // off+112
        };

        // size: 184
        struct TextureD3D12 : public ResourceD3D12 {
            enum class Enum : int {
                Texture2D = 0,
                Texture3D = 1,
                TextureCube = 2,
            };

            D3D12_RESOURCE_DESC              m_desc;            // off+8
            D3D12_SHADER_RESOURCE_VIEW_DESC  m_srvd;            // off+64
            D3D12_UNORDERED_ACCESS_VIEW_DESC m_uavd;            // off+104
            void                            *m_directAccessPtr; // off+144
            uint32_t                         m_flags;           // off+152
            uint32_t                         m_width;           // off+156
            uint32_t                         m_height;          // off+160
            uint32_t                         m_depth;           // off+164
            uint32_t                         m_numLayers;       // off+168
            uint16_t                         m_samplerIdx;      // off+172
            uint8_t                          m_type;            // off+174
            uint8_t                          m_requestedFormat; // off+175
            uint8_t                          m_textureFormat;   // off+176
            uint8_t                          m_numMips;         // off+177
            bool                             m_bHeapAllocated;  // off+178
        };

        // size: 104
        struct FrameBufferD3D12 {
            bgfx::TextureHandle m_texture[8];    // off+0
            bgfx::TextureHandle m_depth;         // off+16
            IDXGISwapChain3    *m_swapChain;     // off+24
            void               *m_nwh;           // off+32
            uint32_t            m_width;         // off+40
            uint32_t            m_height;        // off+44
            uint16_t            m_denseIdx;      // off+48
            uint8_t             m_num;           // off+50
            uint8_t             m_numTh;         // off+51
            bgfx::Attachment    m_attachment[8]; // off+52
            bool                m_needPresent;   // off+100
        };

        // size: 128
        struct VertexBufferD3D12 : public BufferD3D12 {
            bgfx::VertexDeclHandle m_decl; // off+120
        };

        // size: 184
        struct ShaderD3D12 {
            const bgfx::Memory     *m_code;               // off+0
            bgfx::UniformBuffer    *m_dataConstantBuffer; // off+8
            bgfx::PredefinedUniform m_predefined[14];     // off+16

            uint16_t m_attrMask[19]; // off+128
            uint32_t m_hash;         // off+168

            uint16_t m_numUniforms;   // off+172
            uint16_t m_size;          // off+174
            uint8_t  m_numPredefined; // off+176
        };

        // size: 248
        struct ProgramD3D12 {
            const ShaderD3D12      *m_vsh;            // off+0
            const ShaderD3D12      *m_fsh;            // off+8
            bgfx::PredefinedUniform m_predefined[28]; // off+16
            uint8_t                 m_numPredefined;  // off+240
        };

        // size: 256
        class BufferHeapBlock {
        public:
            static constexpr const size_t c_MemBlockGrowCount = 32;
            static constexpr const size_t c_InvalidBlockIndex = -1;

            ID3D12Heap   *m_pHeap;                // off+0
            ID3D12Device *m_pDevice;              // off+8
            HRESULT       m_lastAllocError;       // off+16
            size_t        m_heapSizeInBytes;      // off+24
            size_t        m_alignment;            // off+32
            SPHR_UNINIT(std::mutex m_accessLock;) // off+40

            // size: 48
            struct MemBlock {
                size_t m_start;               // off+0
                size_t m_resourceSizeInBytes; // off+8
                size_t m_blockSizeInBytes;    // off+16
                size_t m_prevMem;             // off+24
                size_t m_nextMem;             // off+32
                size_t m_key;                 // off+40
            };

            std::vector<MemBlock> m_blocks;         // off+120
            std::vector<size_t>   m_inactiveBlocks; // off+144

            using FreeBlock = std::pair<size_t, size_t>;
            using FreeBlocks = std::set<FreeBlock>;

            SPHR_UNINIT(FreeBlocks m_freeBlocks;) // off+168

            using ResourceKey = size_t;
            using UsedBlock = std::pair<size_t, size_t>;
            using UsedBlocks = std::map<size_t, size_t>;

            SPHR_UNINIT(UsedBlocks m_usedBlocks;) // off+184

            using FreeQueueEntry = std::pair<size_t, size_t>;
            using FreeQueue = std::multimap<size_t, size_t>;

            SPHR_UNINIT(FreeQueue m_pendingFreedResources;) // off+200

            ID3D12Resource *m_pSharedResource;      // off+216
            size_t          m_totalActualUsedBytes; // off+224
            size_t          m_totalPaddedUsedBytes; // off+232
            size_t          m_totalFreeBytes;       // off+240
            float           m_totalOccupancy;       // off+248

            SDK_API BufferHeapBlock();

            SDK_API BufferHeapBlock *ctor();

            ~BufferHeapBlock() = delete;

            SDK_API bool init(ID3D12Device *pDevice, ID3D12Heap *pHeap, size_t blockSizeInBytes, size_t alignment);

            SDK_API size_t popInactiveBlockIndex();
        };

        // size: 400
        class BufferHeap {
        public:
            std::wstring  m_name;           // off+0
            ID3D12Device *m_pDevice;        // off+32
            HRESULT       m_lastAllocError; // off+40
            SPHR_UNINIT(
                std::mutex              m_accessLock; // off+48
                std::mutex              m_growLock;   // off+128
                std::condition_variable m_needHeaps;  // off+208
            )
            bool                  m_bAllocHeap;                           // off+280
            bool                  m_bTerminateHeapThread;                 // off+281
            std::thread          *m_pHeapFactoryThread;                   // off+288
            size_t                m_currentFence;                         // off+296
            D3D12_HEAP_PROPERTIES m_heapProperties;                       // off+304
            D3D12_HEAP_FLAGS      m_heapFlags;                            // off+324
            size_t                m_blockSizeInBytes;                     // off+328
            size_t                m_alignment;                            // off+336
            uint32_t              m_numInitialBlocks;                     // off+344
            uint32_t              m_maxPreallocBlocks;                    // off+348
            size_t                m_totalMemoryAllocatedForAllBlocks;     // off+352
            size_t                m_totalMemoryUsedAcrossAllBlocks;       // off+360
            size_t                m_totalPaddedMemoryUsedAcrossAllBlocks; // off+368

            using Heaps = std::vector<BufferHeapBlock *>;

            Heaps m_heaps; // off+376

            SDK_API BufferHeap();

            SDK_API BufferHeap *ctor();

            template <bool A>
            SDK_API bool init(
                const wchar_t               *_name,
                ID3D12Device                *device,
                const D3D12_HEAP_PROPERTIES &heapProps,
                D3D12_HEAP_FLAGS             heapFlags,
                size_t                       blockSizeInBytes,
                size_t                       alignment,
                unsigned int                 initialBlocks,
                unsigned int                 maxPreallocBlocks
            );

            template <bool A>
            BufferHeapBlock *allocHeapBlock(size_t blockSize) const;

            template <>
            SDK_API BufferHeapBlock *allocHeapBlock<true>(size_t blockSize) const;

            template <bool A>
            void preallocateHeaps();

            template <>
            SDK_API void preallocateHeaps<true>();

            SDK_API void addHeapBlock(BufferHeapBlock *pHeapBlock);
        };

        SDK_API void setDebugObjectName(ID3D12Object *_object, const char *_format, ...);

        SDK_API const char *getLostReason(HRESULT _hr);

    } // namespace d3d12

} // namespace bgfx