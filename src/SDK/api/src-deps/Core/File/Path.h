#pragma once

#include <string>

namespace Core {

    template <typename T>
    class PathBuffer;

    // size: 32
    using HeapPathBuffer = Core::PathBuffer<std::string>;

    // size: 32
    class PathPart {
    public:
        std::string mUtf8StdString; // off+0
    };

    // size: 32
    class Path {
    public:
        // size: 1
        struct path_less {
            using is_transparent = void;

            bool operator()(const Core::Path &, const Core::Path &) const;
        };

        PathPart mPathPart; // off+0
    };

    template <typename T>
    class PathBuffer {
    public:
        T mContainer;
    };

} // namespace Core