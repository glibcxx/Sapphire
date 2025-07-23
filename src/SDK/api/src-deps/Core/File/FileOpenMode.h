#pragma once

namespace Core {

    // size: 1
    class FileOpenMode {
    public:
        bool mRead     : 1; // off+0
        bool mWrite    : 1; // off+0
        bool mCreate   : 1; // off+0
        bool mTruncate : 1; // off+0
        bool mAppend   : 1; // off+0
        bool mBinary   : 1; // off+0
    };

} // namespace Core