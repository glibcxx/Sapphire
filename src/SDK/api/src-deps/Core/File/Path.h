#pragma once

#include <string>

namespace Core {

    template <typename T>
    class PathBuffer;

    // size: 32
    using HeapPathBuffer = Core::PathBuffer<std::string>;

    template <typename T>
    class PathBuffer {
    public:
        T mContainer;
    };

} // namespace Core