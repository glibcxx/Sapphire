#pragma once

namespace bx {

    // size: 8
    struct AllocatorI {
        // vtb+0
        virtual ~AllocatorI() = 0;

        // vtb+1
        virtual void *realloc(
            void *_ptr, size_t _size, size_t _align = 0, const char *_filePath = nullptr, uint32_t _line = 0
        ) = 0;
    };

} // namespace bx