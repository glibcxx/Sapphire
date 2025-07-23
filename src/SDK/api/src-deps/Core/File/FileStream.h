#pragma once

#include <iostream>
#include <vector>
#include "File.h"
#include "FileOpenMode.h"
#include "FileTypes.h"

namespace Core {

    // size: 160
    class FileStdStreamBuf : public std::streambuf {
    public:
        Core::File                                      mFile;             // off+104
        Core::FileOpenMode                              mFileOpenMode;     // off+120
        static constexpr const std::streambuf::int_type PUT_BACK_SIZE = 4; // no comment
        std::vector<char>                               mBuffer;           // off+128
        Core::FileSize                                  mBufferSize;       // off+152
    };
    static_assert(sizeof(FileStdStreamBuf) == 160);

    // size: 296
    class FileStream : public std::iostream, public virtual std::ios {
    public:
        Core::FileStdStreamBuf mStreamBuffer;   // off+24
        bool                   mLoggingEnabled; // off+184
    };

    // size: 296
    class OutputFileStream : public FileStream, public virtual std::ios {
    };

} // namespace Core
