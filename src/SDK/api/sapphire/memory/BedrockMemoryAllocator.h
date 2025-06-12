#pragma once

#include "macros/Macros.h"
#include "SDK/api/src-deps/Core/Memory/IMemoryAllocator.h"

namespace sapphire::inline memory {

    /*
        与游戏内分配器 Bedrock::Memory::InternalHeapAllocator 实现一致
        可以在`??2@YAPEAX_K@Z`(operator new)找到原实现的虚表
        operator new -> `\xE8\x00\x00\x00\x00\x0F\x10\x05\x00\x00\x00\x00\x48\x89\x05\x00\x00\x00\x00\x48\xC7\x05`
    */
    class BedrockMemoryAllocator : public Bedrock::Memory::IMemoryAllocator {
    public:
        virtual ~BedrockMemoryAllocator() = default;

        static SDK_API BedrockMemoryAllocator &getInstance();

        virtual void *allocate(const size_t size) override {
            return ::malloc(size ? size : 1);
        }

        virtual void release(void *ptr) override {
            ::free(ptr);
        }

        virtual void *alignedAllocate(const size_t size, const size_t alignment) override {
            return ::_aligned_malloc(size ? size : 1, alignment);
        }

        virtual void alignedRelease(void *ptr) override {
            if (ptr) ::_aligned_free(ptr);
        }

        virtual size_t getUsableSize(void *ptr) override { return 0; }

        virtual void logCurrentState() override {}

        virtual void *_realloc(gsl::not_null<void *> oldPtr, size_t newSize) override {
            return ::realloc(oldPtr, newSize ? newSize : 1);
        }
    };

} // namespace sapphire::inline memory
