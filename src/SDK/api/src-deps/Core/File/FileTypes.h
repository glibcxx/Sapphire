#pragma once

#include <cstdint>
#include <ctime>

namespace Core {

    enum class FileType : int {
        File = 0,
        Directory = 1,
        None = 2,
    };

    enum class FileAccessType : int {
        ReadOnly = 0,
        ReadWrite = 1,
        Flush = 2,
    };

    using FileSize = uint64_t;
    using FileSizeChange = int64_t;
    using FilePosition = FileSize;
    using FileTime = time_t;

}; // namespace Core
