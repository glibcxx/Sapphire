#pragma once

#include <memory>

namespace Core {

    class FileImpl;
    class FileSystemImpl;

    // size: 16
    class File {
    public:
        std::unique_ptr<Core::FileImpl>       muptFile;        // off+0
        std::unique_ptr<Core::FileSystemImpl> muptTransaction; // off+8
    };

} // namespace Core