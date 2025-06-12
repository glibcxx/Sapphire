#pragma once

#include <gsl/gsl>

namespace Bedrock::Memory {

    class IMemoryAllocator {
    public:
        // vtb+0
        virtual ~IMemoryAllocator() = default;

        // vtb+1
        virtual void *allocate(const size_t) = 0;

        // vtb+2
        virtual void release(void *) = 0;

        // vtb+3
        virtual void *alignedAllocate(const size_t, const size_t) = 0;

        // vtb+4
        virtual void alignedRelease(void *) = 0;

        // vtb+5
        virtual size_t getUsableSize(void *) = 0;

        // vtb+6
        virtual void logCurrentState() = 0;

        // vtb+7
        virtual void *_realloc(gsl::not_null<void *>, size_t) = 0;
    };

} // namespace Bedrock::Memory