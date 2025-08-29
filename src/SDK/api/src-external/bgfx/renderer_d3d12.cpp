#include "renderer_d3d12.h"
#include "SDK/core/ApiManager.h"

namespace bgfx::d3d12 {

    BufferHeapBlock::BufferHeapBlock() {
        using Hook = sapphire::ApiLoader<
            "\xE8\x00\x00\x00\x00\x48\x8B\xD8\xEB\x00\x33\xDB\x48\x85\xDB"_sig, // 1.21.50
            &BufferHeapBlock::ctor,
            SPHR_FUNCDNAME>;
        (this->*Hook::origin)();
    }

    BufferHeap::BufferHeap() {
        using Hook = sapphire::ApiLoader<
            "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x33\xFF\x0F\x57\xC0\x0F\x11\x01"_sig, // 1.21.50
            &BufferHeap::ctor,
            SPHR_FUNCDNAME>;
        (this->*Hook::origin)();
    }

    bool BufferHeapBlock::init(
        ID3D12Device *pDevice,
        ID3D12Heap   *pHeap,
        uint64_t      blockSizeInBytes,
        uint64_t      alignment
    ) {
        using Hook = sapphire::ApiLoader<
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x84\xC0\x75\x00\x48\x8B\x4C\x24\x00\x48\x8B\x01"_sig, // 1.21.50
            &BufferHeapBlock::init>;
        return (this->*Hook::origin)(pDevice, pHeap, blockSizeInBytes, alignment);
    }

    size_t BufferHeapBlock::popInactiveBlockIndex() {
        using Hook = sapphire::ApiLoader<
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x4C\x8B\xC0\x48\x8B\x5E"_sig, // 1.21.50
            &BufferHeapBlock::popInactiveBlockIndex>;
        return (this->*Hook::origin)();
    }

    // template <bool A>
    // bool BufferHeap::init(
    //     const wchar_t               *_name,
    //     ID3D12Device                *device,
    //     const D3D12_HEAP_PROPERTIES &heapProps,
    //     D3D12_HEAP_FLAGS             heapFlags,
    //     size_t                       blockSizeInBytes,
    //     size_t                       alignment,
    //     unsigned int                 initialBlocks,
    //     unsigned int                 maxPreallocBlocks
    // ) {
    //     std::lock_guard lock{this->m_accessLock};
    //     if constexpr (A) {
    //         this->m_name = L"AccelerationStructureBufferHeap";
    //         this->m_pDevice = device;
    //         this->m_heapProperties = heapProps;
    //         this->m_heapFlags = D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;
    //         this->m_numInitialBlocks = 8;
    //         this->m_maxPreallocBlocks = 4;
    //         this->m_blockSizeInBytes = 0x800000;
    //         this->m_alignment = 0x100;
    //         for (size_t i = initialBlocks; i > 0; --i) {
    //             if (BufferHeapBlock *pHeapBlock = allocHeapBlock<true>(0x800000))
    //                 addHeapBlock(pHeapBlock);
    //         }
    //     } else {
    //         this->m_name = _name;
    //         this->m_pDevice = device;
    //         this->m_heapProperties = heapProps;
    //         this->m_heapFlags = D3D12_HEAP_FLAG_NONE;
    //         this->m_numInitialBlocks = initialBlocks;
    //         this->m_maxPreallocBlocks = maxPreallocBlocks;
    //         this->m_blockSizeInBytes = blockSizeInBytes;
    //         this->m_alignment = 0x10000;
    //         for (size_t i = initialBlocks; i > 0; --i) {
    //             if (BufferHeapBlock *pHeapBlock = allocHeapBlock<true>(blockSizeInBytes))
    //                 addHeapBlock(pHeapBlock);
    //         }
    //     }
    //     this->m_bAllocHeap = false;
    //     this->m_bTerminateHeapThread = false;
    //     this->m_pHeapFactoryThread = new std::thread(&BufferHeap::preallocateHeaps<true>, this);
    // }

    template <>
    bool BufferHeap::init<true>(
        const wchar_t               *_name,
        ID3D12Device                *device,
        const D3D12_HEAP_PROPERTIES &heapProps,
        D3D12_HEAP_FLAGS             heapFlags,
        size_t                       blockSizeInBytes,
        size_t                       alignment,
        unsigned int                 initialBlocks,
        unsigned int                 maxPreallocBlocks
    ) {
        using Hook = sapphire::ApiLoader<
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x84\xC0\x74\x00\x48\x89\x9F\x00\x00\x00\x00\x48\xC7\x45"_sig, // 1.21.50
            &BufferHeap::init<true>>;
        return (this->*Hook::origin)(_name, device, heapProps, heapFlags, blockSizeInBytes, alignment, initialBlocks, maxPreallocBlocks);
    }

    template <>
    bool BufferHeap::init<false>(
        const wchar_t               *_name,
        ID3D12Device                *device,
        const D3D12_HEAP_PROPERTIES &heapProps,
        D3D12_HEAP_FLAGS             heapFlags,
        size_t                       blockSizeInBytes,
        size_t                       alignment,
        unsigned int                 initialBlocks,
        unsigned int                 maxPreallocBlocks
    ) {
        using Hook = sapphire::ApiLoader<
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x84\xC0\x74\x00\x48\x89\x9F\x00\x00\x00\x00\xEB"_sig, // 1.21.50
            &BufferHeap::init<false>>;
        return (this->*Hook::origin)(_name, device, heapProps, heapFlags, blockSizeInBytes, alignment, initialBlocks, maxPreallocBlocks);
    }

    template <>
    BufferHeapBlock *BufferHeap::allocHeapBlock<true>(size_t blockSize) const {
        using Hook = sapphire::ApiLoader<
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\xE8\x48\x85\xC0\x74\x00\x48\x8D\x5F"_sig, // 1.21.50
            &BufferHeap::allocHeapBlock<true>>;
        return (this->*Hook::origin)(blockSize);
    }

    template <>
    void BufferHeap::preallocateHeaps<true>() {
        using Hook = sapphire::ApiLoader<
            sapphire::deRefLea | "\x48\x8D\x0D\x00\x00\x00\x00\x48\x89\x48\x00\xEB\x00\x49\x8B\xC7\x48\x89\x44\x24"_sig, // 1.21.50
            &BufferHeap::preallocateHeaps<true>>;
        (this->*Hook::origin)();
    }

    void BufferHeap::addHeapBlock(BufferHeapBlock *pHeapBlock) {
        using Hook = sapphire::ApiLoader<
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x83\xEF\x00\x75\x00\x66\x89\xBE"_sig, // 1.21.50
            &BufferHeap::addHeapBlock>;
        (this->*Hook::origin)(pHeapBlock);
    }

    void setDebugObjectName(ID3D12Object *_object, const char *_format, ...) {
        using Hook = sapphire::ApiLoader<
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xEB\x00\x45\x33\xE4\x4C\x89\xA7"_sig, // 1.21.50
            &setDebugObjectName>;
        Hook::origin(_object, _format);
        // we can't forward variant args, but we don't need to, because it's an empty function.
    }

    const char *getLostReason(HRESULT _hr) {
        using Hook = sapphire::ApiLoader<
            "\x81\xF9\x20\x00\x7A\x88\x7F\x00\x74\x00\x81\xF9"_sig,
            &getLostReason>;
        return Hook::origin(_hr);
    }

} // namespace bgfx::d3d12